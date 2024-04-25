#include "mtp.h"
#include "w1distrib.h"
#include <algorithm>
/**
 * MTP source first
*/

/**
 * Define static variable values for the linker
*/
// You MUST set a route strategy.  The default is to abort without
// running - this is deliberate!
MtpRouteStrategy MtpSrc::_route_strategy = MtpRouteStrategy::NOT_SET;
MtpRouteStrategy MtpSink::_route_strategy = MtpRouteStrategy::NOT_SET;

#define MTP_SENDERLOG_DELAY (uint32_t) 20
/**
 * 1. Timer retry
 * 2. Sending up to cwnd
 * 3. ARX handling
 * 4. ACK handling
 **/
MtpSrc::MtpSrc(MtpLogger* logger, TrafficLogger* pktlogger, EventList &eventlist, linkspeed_bps bitrate, char* lfile,
bool immediate, uint64_t offered_load)
    : EventSource(eventlist, "mtp_src"), _logger(logger), _flow(pktlogger), channels(MAX_CHANNELS), 
    CWND(2), in_flight(0), deadlines(MAX_CHANNELS), next_msg_id(0), _flow_started(false), _bitrate(bitrate),
    _offered_load(speedFromMbps((double) offered_load)/(8 * Packet::data_packet_size()))
{ 
    _ps_per_byte = (simtime_picosec)((pow(10.0, 12.0) * 8) / _bitrate);
    _nodename = "MTPSRC";
    for(int i = 0; i < MAX_CHANNELS; i++){
        deadlines[i] = UINT64_MAX;
    }

    senderlog = fopen(lfile, "wS");
    if (ferror(senderlog)) {
        perror("fopen");
    }
    fprintf(senderlog, "ts,cwnd,rtx,txq,if,rcvd\n");
    nextWrite = eventlist.now() + timeFromUs(MTP_SENDERLOG_DELAY);
    eventlist.sourceIsPending(*this, nextWrite);
}


bool MtpSrc::invariant_check(){
    return in_flight + msgQueue.size() == 256;
}


void MtpSrc::connect(Route& routeout, Route& routeback, MtpSink& sink, simtime_picosec starttime){
    _route = &routeout;
    _sink = &sink;
    _start = starttime;
    assert(_route);
    _sink->connect(*this, routeback);
    eventlist().sourceIsPending(*this, starttime);
}

void MtpSrc::startflow(){
    for(size_t channel_id = 0; channel_id < MAX_CHANNELS; channel_id++){
        create_message(channel_id);
        msgQueue.push_back(channel_id);
    }

    assert(msgQueue.size()==256);

    while(in_flight < floor(CWND)){
        int32_t channel_id = msgQueue.front();
        msgQueue.pop_front();
        send_message(channel_id);
    }
    assert(invariant_check());
}


void MtpSrc::set_paths(vector<const Route*>* rt_list){
    _paths = *rt_list;
}

void MtpSrc::send_message(seq_t channel_id){
    set<MtpPacket*> message = channels[channel_id];

    // must receive ARX by this deadline
    deadlines[channel_id] = eventlist().now() + SHORT_FABRIC_RTO;
    eventlist().sourceIsPendingRel(*this, SHORT_FABRIC_RTO);
    
    // cout << "sending size: " << message.size() << " channel: " << channel_id << endl;
    in_flight++;
    bool first = txQueue.size() == 0;
    for(MtpPacket* pkt: message){
        MtpPacket* cpypkt = MtpPacket::cpypkt(pkt);
        cpypkt->set_ts(eventlist().now());
        txQueue.push_back(cpypkt);
    }
    if(first){
        nextDrain = drainTime(txQueue.front())+eventlist().now();
        eventlist().sourceIsPending(*this, nextDrain);
    }
    assert(invariant_check());
    // cout << in_flight << " " << CWND << " " << timeAsUs(eventlist().now()) << endl;
}

void MtpSrc::create_message(seq_t channel_id){
    assert(_route_strategy != MtpRouteStrategy::NOT_SET);
    channels[channel_id].clear();

    // generate message size
    mem_b ttl_pkts = w5distrib::sample();
    int32_t msg_id = next_msg_id++;


    // create packets in message
    for(int32_t pidx = 0; pidx < ttl_pkts; pidx++){
        MtpPacket* pkt = MtpPacket::newpkt(_flow, pidx, ttl_pkts, msg_id, id, _sink->id, channel_id, 
        Packet::data_packet_size(), msg_id, unordered_map<size_t, mem_b>());
        
        // set route from strategy
        switch(_route_strategy){
            case MtpRouteStrategy::SINGLE_PATH:{
                pkt->set_route(*_route);
                break;
            }
            case MtpRouteStrategy::SCATTER_ROUNDROBIN:
                pkt->set_route(*(_paths[++rr_ctr%_paths.size()]));
                break;
            case MtpRouteStrategy::SCATTER_RANDOM:
                pkt->set_route(*(_paths[rand()%_paths.size()]));
                break;
            default:
                break;
        }

        channels[channel_id].insert(pkt);
    }
}

/** Free message */
void MtpSrc::free_message(seq_t channel_id){
    set<MtpPacket*> channel = channels[channel_id];
    for(MtpPacket* pkt: channel){
        pkt->free();
    }
}

void MtpSrc::receivePacket(Packet& pkt){
    assert(invariant_check());
    switch(pkt.type()){
        case MTPACK:{
            MtpAck& ack = dynamic_cast<MtpAck&>(pkt);

            // handle dup acks
            if((*channels[ack.get_channel_id()].begin())->get_message_id() != ack.get_message_id()){
                ack.free();
                return;
            }

            deadlines[ack.get_channel_id()] = UINT64_MAX;

            // remove any rtx from queue
            for(auto it = msgQueue.begin(); it != msgQueue.end();){
                if(*it == ack.get_channel_id()){
                    it = msgQueue.erase(it);
                    in_flight++; // undo the drop from earlier
                }else{
                    it++;
                }
            }
            
            if(filling_pathlets.empty()){
                CWND += 1.0/floor(CWND);
                CWND = std::min(CWND, MAX_CHANNELS);
            }
            assert(invariant_check());
            total_rcvd++;
            total_size+=channels[ack.get_channel_id()].size();
            free_message(ack.get_channel_id());
            create_message(ack.get_channel_id());
            msgQueue.push_back(ack.get_channel_id());
            in_flight--;
            assert(invariant_check());
            while(in_flight < floor(CWND)){
                size_t channel_id = msgQueue.front();
                msgQueue.pop_front();
                send_message(channel_id);
                assert(invariant_check());
            }
            ack.free();
            assert(invariant_check());
            break;
        }
        case MTPARX:{
            MtpArx& arx = dynamic_cast<MtpArx&>(pkt);

            if(!arx.dropped()){
                deadlines[arx.get_channel_id()] = eventlist().now() + LONG_PATHLET_RTO;
                eventlist().sourceIsPendingRel(*this, LONG_PATHLET_RTO);
                // cout << "arx: " << arx.get_channel_id() << " " << deadlines[arx.get_channel_id()] << endl;
            }
            
            // any threshold cross or dropping in a pathlet
            if(arx.get_slowdown() || arx.dropped()){
                // reaction to network slowdown
                CWND/=2;
                CWND = std::max(floor(CWND), (double) 1);
                filling_pathlets.insert(arx.get_pathlet_id());
            } else {
                filling_pathlets.erase(arx.get_pathlet_id());
            }

            // if txqueue is free, remove the block
            if(!arx.get_tx_slowdown()){
                filling_pathlets.erase(-1* arx.get_pathlet_id());
            }

            // after saving sizes, retransmit if dropped and in channel
            if(arx.dropped()){
                // msg retransmit, if channel is less than CWND:
                // if drop, retransmit (if in CWND)
                // if just threshold cross, don't retransmit
                // TODO: can cause stack overflow
                // cout << "retransmitting 1" << endl;
                in_flight--;
                msgQueue.push_front(arx.get_channel_id());
                if(in_flight < floor(CWND)){
                    size_t rtx_channel = msgQueue.front();
                    msgQueue.pop_front();
                    send_message(rtx_channel);
                }
            }

            arx.free();
            assert(invariant_check());
            break;
        }
        case MTPATX:{
            MtpAtx& atx = dynamic_cast<MtpAtx&>(pkt);

            if(!atx.get_tx_dropped()){
                // must receive ARX or ACK by this deadline
                deadlines[atx.get_channel_id()] = eventlist().now() + SHORT_FABRIC_RTO;
                eventlist().sourceIsPendingRel(*this, SHORT_FABRIC_RTO);
                // cout << "atx: " << atx.get_channel_id() << " " << deadlines[atx.get_channel_id()] << endl;
            }

            // slowdown network if txqueue is filling
            if(atx.get_tx_slowdown()){
                // reaction to network slowdown
                CWND/=2;
                CWND = std::max(floor(CWND), (double) 1);
                filling_pathlets.insert(-1*atx.get_pathlet_id());
            }else{
                filling_pathlets.erase(-1*atx.get_pathlet_id());
            }

            // copy size history into all packets in message, if new history
            if(atx.get_sizes().size() >= (*channels[atx.get_channel_id()].begin())->get_sizes().size()){
                set<MtpPacket*> msg  = channels[atx.get_channel_id()];
                for(MtpPacket* pkt: msg){
                    pkt->set_sizes(atx.get_sizes());
                }
            }

            // INC RX queue freeing
            if(!atx.get_slowdown()){
                filling_pathlets.erase(atx.get_pathlet_id());
            }

            // after saving sizes, retransmit if dropped and in channel
            if(atx.get_tx_dropped()){
                // msg retransmit, if channel is less than CWND:
                // if drop, retransmit (if in CWND)
                // if just threshold cross, don't retransmit
                // TODO: can cause stack overflow
                // cout << "retransmitting 3" << endl;
                in_flight--;
                msgQueue.push_front(atx.get_channel_id());
                if(in_flight < floor(CWND)){
                    size_t rtx_channel = msgQueue.front();
                    msgQueue.pop_front();
                    send_message(rtx_channel);
                }
            }

            atx.free();
            assert(invariant_check());
            break;
        }
        default:
            break;
    }
}

// void MtpSrc::offer_load(){
//     uint64_t mult = 10e6/OFFER_INTERVAL;
//     uint64_t packets = _offered_load * mult/10e6;

//     while(packets > 0){
//         mem_b ttl_pkts = w5distrib::sample();
//         packets-=ttl_pkts;
//     }
// }

void MtpSrc::doNextEvent(){
    if(!_flow_started && eventlist().now() == _start){
        _flow_started = true;
        startflow();
    }else{
        // check for timers -> then retransmit
        simtime_picosec now = eventlist().now();
        for(size_t channel = 0; channel < MAX_CHANNELS; channel++){
            if(deadlines[channel] <= now){
                deadlines[channel] = UINT64_MAX;
                _total_rtxs++;
                /** Should never happen, unless single packet message drops*/
                // TODO: Update CWND here?
                // cout << "retransmitting 2 channel: " << channel << " " << eventlist().now() << " " << deadlines[channel] << endl;
                // Retransmission
                msgQueue.push_front(channel);
                in_flight--;
                if(in_flight < floor(CWND) || in_flight == 0){
                    size_t rtx_channel = msgQueue.front();
                    msgQueue.pop_front();
                    send_message(rtx_channel);
                }
                assert(invariant_check());
            }
        }
    }

    // drain
    if(nextDrain == eventlist().now()){
        assert(txQueue.size());
        MtpPacket* pkt = txQueue.front();
        txQueue.pop_front();
        pkt->sendOn();
        if(txQueue.size()){
            nextDrain = eventlist().now() + drainTime(txQueue.front());
            eventlist().sourceIsPending(*this, nextDrain);
        }else{
            nextDrain = 0;
        }
    }

    if(nextWrite == eventlist().now()){
        fprintf(senderlog, "%f,%d,%d,%d,%d,%d\n", timeAsMs(eventlist().now()), (int)CWND, (int)_total_rtxs, (int)txQueue.size(), (int)in_flight, (int) total_rcvd);
        nextWrite = eventlist().now() + timeFromUs(MTP_SENDERLOG_DELAY);
        eventlist().sourceIsPending(*this, nextWrite);
    }
    
}



MtpSink::MtpSink(EventList& eventlist)
    : EventSource(eventlist, "mtp_sink"), _total_received(0), last_fully_rcvd_msg(MAX_CHANNELS), cur_msgs(MAX_CHANNELS), _ack_cooldowns(MAX_CHANNELS, 0), _used_channels(MAX_CHANNELS, false)
{
    _nodename = "MTPSINK";
}


void MtpSink::receivePacket(Packet& pkt){
    MtpPacket& mtppkt = dynamic_cast<MtpPacket&>(pkt);

    seq_t channel_id = mtppkt.get_channel_id();
    int32_t message_id = mtppkt.get_message_id();

    bool sendAck = true;
    MtpAck* ack;

    // retransmit duplicate messsage
    if(last_fully_rcvd_msg[channel_id] && last_fully_rcvd_msg[channel_id]->get_message_id() == message_id){
        if(_ack_cooldowns[channel_id] <= eventlist().now() - ACK_COOLDOWN){
            ack = MtpAck::cpypkt(last_fully_rcvd_msg[channel_id]);
            _ack_cooldowns[channel_id] = eventlist().now();
        }else{
            sendAck = false; // cooldown check not passed
        }
    }else{
        cur_msgs[channel_id].insert(MtpPacket::cpypkt(&mtppkt));
        size_t message_pkts = mtppkt.get_ttl_pkts();
        // cout << channel_id << ": rcvd " << cur_msgs[channel_id].size() << std::endl;
        if(cur_msgs[channel_id].size() == message_pkts){
            for(MtpPacket* pkt: cur_msgs[channel_id]){
                pkt->free();
            }
            cur_msgs[channel_id].clear();
            ack = MtpAck::newpkt(_src->_flow, mtppkt.get_message_id(), mtppkt.get_src(), mtppkt.get_dest(), mtppkt.get_channel_id(), eventlist().now(), mtppkt.id());
            if(_used_channels[channel_id]){
                last_fully_rcvd_msg[channel_id]->free();
            }
            last_fully_rcvd_msg[channel_id] = MtpAck::cpypkt(ack);
            _used_channels[channel_id] = true;
            
            _total_received++;
        }else{
            sendAck = false;
        }
    }

    mtppkt.free();

    if(sendAck){
        assert(_route_strategy != MtpRouteStrategy::NOT_SET);
        switch(_route_strategy){
            case MtpRouteStrategy::SINGLE_PATH:
                ack->set_route(*_route);
                break;
            case MtpRouteStrategy::SCATTER_ROUNDROBIN:
                ack->set_route(*_paths[++rr_ctr%_paths.size()]);
                break;
            case MtpRouteStrategy::SCATTER_RANDOM:
                ack->set_route(*_paths[rand()%_paths.size()]);
                break;
            default:
                break;
        }
        ack->sendOn();
    }

    
}

void MtpSink::connect(MtpSrc& src, Route& route){
    assert(_route_strategy != MtpRouteStrategy::NOT_SET);
    _src = &src;
    switch(_route_strategy){
        case MtpRouteStrategy::SINGLE_PATH:
            _route = &route;
            break;
        default:
            _route = nullptr;
            break;
    }
}

void MtpSink::set_paths(vector<const Route*>* rt_list){
    _paths = *rt_list;
}