#include "msgqueueog.h"
#include "mtppacket.h"
#include "w1distrib.h"
#include "w5distrib.h"
#include "route.h"
#include <algorithm>
#include <sstream>

// This class only works with MTP Packets
#define MESSAGE_RCV_DEADLINE (uint64_t) 28e9
#define MICROSECONDS_TO_PICOSECONDS (uint64_t) 1000000

MsgQueueOg::MsgQueueOg(linkspeed_bps bitrate, mem_b maxsize,
    EventList &eventlist, AbstractQueueLogger* logger, string fp)
    : AbstractQueue(bitrate, maxsize, eventlist, logger), processingDistribution(PROCESSING_TIME_MEAN) //, sizelog("mtpog_qsize.txt")
{
    logsize = fopen(fp.c_str(), "wS");
    if (ferror(logsize)) {
        perror("fopen");
    }
    fprintf(logsize, "ts,rx,tx,rq,exp,drops\n");

    stringstream ss;
    ss << "MsgQueueOg(" << bitrate/10000000 << "Mb/s," << maxsize << "bytes)";
    _nodename = ss.str();
    _maxpkts = maxsize/Packet::data_packet_size();
    threshold_size =  (uint32_t) (((double)_maxpkts) * MsgQueueOg_THRESHOLD);
    nextWrite = eventlist.now() + timeFromUs(MTPOG_QSIZE_DELAY);
    eventlist.sourceIsPending(*this, nextWrite);
}

void MsgQueueOg::receivePacket(Packet& pkt){
    switch(pkt.type()){
        case MTP:{
            // convert packet to an MTP Packet
            MtpPacket& mtppkt = dynamic_cast<MtpPacket&>(pkt);

            // assert(mtppkt.nexthop() == 1);
            uint64_t store_idx = (((uint64_t)mtppkt.get_src()) << 32) + mtppkt.get_message_id();
            mem_b msg_pkt_len = mtppkt.get_ttl_pkts();

            // cout << ttl_pkts << " " << timeAsUs(eventlist().now()) << endl;
            // if message doesn't exist
            if(!msgStore.count(store_idx)){
                // cout << mtppkt.get_channel_id() << " ttlpkts: " <<  ttl_pkts << " msgpkts: " << msg_pkt_len << endl;
                // if message won't fit
                if(ttl_pkts + msg_pkt_len > _maxpkts){
                    // hack for logging message drop
                    if(mtppkt.get_pkt_seqno() == 0){
                        _ttl_drops++;
                        // _ttl_rx_drops++;
                        // cout << _ttl_rx_drops << endl;
                    }
                    
                    // log the drop
                    if(_logger){
                        ((MsgQueueLogger*) _logger)->logQueue(*this, MsgQueueLogger::MsgQueueEvent::MSG_DROP, pkt);
                    }

                    mtppkt.free();
                    return;
                }

                // alloc space in mem for this full msg
                ttl_pkts += msg_pkt_len;
                assert(ttl_pkts <= _maxpkts);

                // log the start of enqueuing and allocing space
                if(_logger){
                    ((MsgQueueLogger*) _logger)->logQueue(*this, MsgQueueLogger::MsgQueueEvent::MSG_ENQUEUE, pkt);
                }

                // if not last packet
                if(msg_pkt_len != 1){
                    msgStore[store_idx].deadline = eventlist().now() + MESSAGE_RCV_DEADLINE;
                    eventlist().sourceIsPendingRel(*this, MESSAGE_RCV_DEADLINE);
                }
            }
            msgStore[store_idx].packets.insert(&mtppkt);
            assert(msgStore[store_idx].packets.size());
            
            // if this is the last packet for a message
            if(msgStore[store_idx].packets.size() == msg_pkt_len){
                // cout << "last: " << mtppkt.get_channel_id() << endl;
                // cout << "RQ: " << readyq.size() << " TXQ: " << txQueue.size() << " RXQ: " << ttl_pkts <<  endl;

                // delete message from store
                set<MtpPacket*> msg_pkts = msgStore[store_idx].packets;
                msgStore.erase(store_idx);
                
                // ready queue
                if(readyq.empty()){
                    uint32_t procTime = processingDistribution(rd);
                    //cout << "PT: " << procTime << "| " << mtppkt.get_message_id() << endl;
                    procEndTime = eventlist().now() + procTime * MICROSECONDS_TO_PICOSECONDS;
                    eventlist().sourceIsPendingRel(*this, procTime * MICROSECONDS_TO_PICOSECONDS);
                }

                // for(MtpPacket* pkt: msg_pkts){
                //     assert(pkt->nexthop() == 1);
                // }
                readyq.push_back(msg_pkts);

                // send an MTP ARX back
                uint8_t feedback = 256* ((double)ttl_pkts/(double) _maxpkts);
                MtpArx* arx = MtpArx::newpkt(mtppkt.flow(), mtppkt.get_message_id(), mtppkt.nexthop(), mtppkt.get_src(), mtppkt.get_channel_id(), eventlist().now(), mtppkt.get_message_id(), ttl_pkts >= threshold_size, false, feedback, msg_pkt_len, txQueue.size() >= threshold_size);
                if(mtppkt.get_ecn()){
                    arx->set_ecn();
                }
                arx->set_route(*mtppkt.route());
                arx->set_nexthop(mtppkt.nexthop());
                arx->bounce();
                arx->sendOn();
            }
            break;
        }
        default:{
            /**
             * No need to buffer or process, just send all acks back immediately
            */
            // pkt.sendOn();
            cout << pkt.str() << endl;
            cout << "HUHHHH?! Expected no other packet type" << endl;
            break;
        }
    }

}

/**
 * 3 events to handle:
 * 1. handle finished message processing
 * 2. handle expired messages
 * 3. handle transmitting messages
*/
void MsgQueueOg::doNextEvent(){
    // 1) handle finished message processing
    if(!readyq.empty() && procEndTime <= eventlist().now()){
        MtpPacket* mtppkt = (*(readyq.begin()->begin()));
        // assert(mtppkt->nexthop() == 1);

        ttl_pkts -= mtppkt->get_ttl_pkts();
        assert(ttl_pkts < _maxpkts);
        assert(ttl_pkts >= 0);

        // add size info
        unordered_map<size_t, mem_b> sizes = mtppkt->get_sizes();

        size_t newMsgSize;
        // compute a resize if one doesn't exist for this message
        if(sizes.count(mtppkt->nexthop())){
            newMsgSize = (size_t) sizes[mtppkt->nexthop()];
        }else{
            // sample from resize distribution and create new msgsize in bytes
            newMsgSize = w5distrib::sample()*Packet::data_packet_size();
            sizes[mtppkt->nexthop()] = newMsgSize;
        }
        mem_b pkts = std::max((size_t) 1, newMsgSize/Packet::data_packet_size());
    
        if(_logger){
            ((MsgQueueLogger*) _logger)->logQueue(*this, MsgQueueLogger::MsgQueueEvent::MSG_PROCESSING, *(*txQueue.end()));
        }
        bool drop  = true;
        // prevent txqueue from filling
        if(pkts + txQueue.size() < _maxpkts){
            drop = false;
            // create msg and send
            for(mem_b pkt_idx = 0; pkt_idx < pkts; pkt_idx ++){
                MtpPacket* newPkt = MtpPacket::newpkt(mtppkt->flow(), mtppkt->get_pkt_seqno(), pkts,
                mtppkt->get_message_id(), mtppkt->get_src(), mtppkt->get_dest(), mtppkt->get_channel_id(), Packet::data_packet_size(), 
                mtppkt->get_message_id(), sizes);
                newPkt->set_ts(mtppkt->ts());
                newPkt->set_route(*mtppkt->route());
                newPkt->set_nexthop(mtppkt->nexthop());

                if(txQueue.empty()){
                    nextDrainTime = drainTime(newPkt) + eventlist().now();
                    eventlist().sourceIsPending(*this, nextDrainTime);
                }
                txQueue.push_back(newPkt);
            }

            if(_logger){
                ((MsgQueueLogger*) _logger)->logQueue(*this, MsgQueueLogger::MsgQueueEvent::MSG_SERVICE, *(*txQueue.end()));
            }
        }

        if(drop){
            _ttl_drops++;
            // _ttl_tx_drops++;
            // cout << _ttl_tx_drops << endl;
        }

        

        // send back MTP ATX
        // cout << ttl_pkts << " " << mtppkt->get_message_id() << " " << timeAsUs(eventlist().now()) << endl;
        MtpAtx* atx = MtpAtx::newpkt(mtppkt->flow(), mtppkt->get_message_id(), mtppkt->nexthop(), mtppkt->get_src(), mtppkt->get_channel_id(), eventlist().now(), mtppkt->get_message_id(), ttl_pkts >= threshold_size, drop, txQueue.size() >= threshold_size);
        atx->set_route(*mtppkt->route());
        atx->set_nexthop(mtppkt->nexthop());
        atx->set_sizes(sizes);
        atx->bounce();
        atx->sendOn();

        // free old packets
        set<MtpPacket*> oldMsg = *readyq.begin();
        for(MtpPacket* oldPkt: oldMsg){
            oldPkt->free();
        }
        readyq.erase(readyq.begin());

        // next message to process
        if(!readyq.empty()){
            uint32_t procTime = processingDistribution(rd);
            //cout << "PT: " << procTime << "| " << (*(*readyq.begin()).begin())->get_message_id() << endl;
            procEndTime = eventlist().now() + procTime * MICROSECONDS_TO_PICOSECONDS;
            eventlist().sourceIsPendingRel(*this, procTime * MICROSECONDS_TO_PICOSECONDS);
        }

        // cout << "RQ: " << readyq.size() << " TXQ: " << txQueue.size() << " RXQ: " << ttl_pkts <<  endl;
    } 

    // 2) handle expired messages
    for(auto storeEntry = msgStore.begin(); storeEntry != msgStore.end(); ){
        MtpOgMsgStore store = storeEntry->second;

        // unresolved and past deadline
        if(store.deadline <= eventlist().now()){
            MtpPacket* mtppkt = *(store.packets.begin());

            ttl_pkts -= mtppkt->get_ttl_pkts();
            
            assert(ttl_pkts < _maxpkts);
            assert(ttl_pkts >= 0);

            for(MtpPacket* expPacket: store.packets){
                expPacket->free();
            }
            storeEntry = msgStore.erase(storeEntry);
            _ttl_expirations++;
            if(_logger){
                ((MsgQueueLogger*) _logger)->logQueue(*this, MsgQueueLogger::MsgQueueEvent::MSG_EXPIRE, *mtppkt);
            }
        }else{
            storeEntry++;
        }
    }

    // 3) handle tx queue drain
    if(eventlist().now() == nextDrainTime){
        (*(txQueue.begin()))->sendOn();
        txQueue.pop_front();
        if(!txQueue.empty()){
            nextDrainTime = drainTime(*txQueue.begin()) + eventlist().now();
            eventlist().sourceIsPending(*this, nextDrainTime);
        }else{
            nextDrainTime = 0;
        }
    }
    
    if(nextWrite == eventlist().now()){
        fprintf(logsize, "%f,%d,%d,%d,%d,%d\n", timeAsMs(eventlist().now()), (int)ttl_pkts, (int)txQueue.size(), (int)readyq.size(), _ttl_expirations, _ttl_drops);
        nextWrite = eventlist().now() + timeFromUs(MTPOG_QSIZE_DELAY);
        eventlist().sourceIsPending(*this, nextWrite);
    }
}