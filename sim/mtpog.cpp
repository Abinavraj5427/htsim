#include "mtpog.h"
#include "w1distrib.h"
#include <limits>
#include <algorithm>
#include <unordered_set>
/**
 * MTP source first
*/

/**
 * Define static variable values for the linker
*/
// You MUST set a route strategy.  The default is to abort without
// running - this is deliberate!
MtpRouteStrategy MtpSrcOg::_route_strategy = MtpRouteStrategy::NOT_SET;
MtpRouteStrategy MtpSinkOg::_route_strategy = MtpRouteStrategy::NOT_SET;

#define INIT_CWND 3000
#define MTPOG_SENDERLOG_DELAY (uint32_t) 20
/**
 * 1. Timer retry
 * 2. Sending up to cwnd
 * 3. ARX handling
 * 4. ACK handling
 **/
MtpSrcOg::MtpSrcOg(MtpLogger* logger, TrafficLogger* pktlogger, EventList &eventlist, linkspeed_bps bitrate, string fp)
    : EventSource(eventlist, "mtp_src"), _logger(logger), _flow(pktlogger), channels(MAX_CHANNELS, std::set<MtpPacket*>()),
    in_flight(0), deadlines(MAX_CHANNELS, UINT64_MAX), _flow_started(false), _bitrate(bitrate)
{ 
    // assert(channels[0].size() == 0);
    _ps_per_byte = (simtime_picosec)((pow(10.0, 12.0) * 8) / _bitrate);
    next_msg_id = 0;
    _nodename = "MtpSrcOg";
    senderlog = fopen(fp.c_str(), "wS");
    if (ferror(senderlog)) {
        perror("fopen");
    }
    fprintf(senderlog, "ts,cwnd,rtx,txq,if,rcvd\n");
    nextWrite = eventlist.now() + timeFromUs(MTPOG_SENDERLOG_DELAY);
    eventlist.sourceIsPending(*this, nextWrite);
}

bool MtpSrcOg::invariant_check(){
    size_t totalSizes = 0;
    size_t msgQueueSizes = 0;
    for(auto channel: msgQueue){
        msgQueueSizes += channels[channel].size();
    }
    for(auto msg: channels){
        totalSizes += msg.size();
    }
    return (in_flight + msgQueueSizes) == totalSizes;
}

bool MtpSrcOg::invariant_check2(){
    unordered_set<size_t> msgSet(msgQueue.begin(), msgQueue.end());
    size_t exp_in_flight = 0;
    for(size_t channel = 0; channel < MAX_CHANNELS; channel++){
        if(!msgSet.count(channel)){
            exp_in_flight += channels[channel].size();
        }
    }
    return exp_in_flight == in_flight;
}

void MtpSrcOg::invariant_check3(){
    for(auto channel: msgQueue){
        assert(deadlines[channel] = UINT64_MAX);
    }
}


void MtpSrcOg::connect(Route& routeout, Route& routeback, MtpSinkOg& sink, simtime_picosec starttime){
    _route = &routeout;
    _sink = &sink;
    assert(_route);
    _sink->connect(*this, routeback);
    eventlist().sourceIsPending(*this, starttime);
}

double MtpSrcOg::min_CWND(){
    double CWND = !CWNDs.size()? INIT_CWND: std::numeric_limits<double>::max();
    for(auto pair: CWNDs){
        double candidate = pair.second;
        CWND = std::min(candidate, CWND);
    }
    CWND = std::min(link_CWND, CWND);
    return CWND;
}

void MtpSrcOg::update_CWNDs(){
    alpha = (1-g)*alpha + g*(ecn_seen/total_seen);
    if(ecn_seen/total_seen < 0.5){
        link_CWND +=1;
    }else{
        link_CWND = link_CWND * (1-alpha/2);
    }
    ecn_seen = 0;
    total_seen = 0;

    for(auto pair : CWNDs){
        size_t pid = pair.first;
        double CWND = pair.second;
        // no feedback to update on
        double rt = round_total[pid];
        double rce = round_ces[pid];
        if(rt == 0)
            continue;
        double cong_ratio = rce/rt;
        if(cong_ratio > 0.5){
            CWNDs[pid] = (0.2 + 0.8*(rt/(2*rce)))*CWND;
            assert(CWND > CWNDs[pid]);
        }else{
            CWNDs[pid] = (1.0+ (rt/CWND)) * CWND;
        }
        CWNDs[pid] = std::max((double) 1, CWNDs[pid]);
        round_ces[pid] = 0;
        round_total[pid] = 0;
    }
}

void MtpSrcOg::startflow(){
    for(size_t channel_id = 0; channel_id < MAX_CHANNELS; channel_id++){
        create_message(channel_id);
        msgQueue.push_back(channel_id);
    }

    // assert(msgQueue.size()==256);

    // send based on CWND
    size_t CWND = floor(min_CWND());

    auto it = msgQueue.begin();
    while(msgQueue.size() && in_flight < CWND && it != msgQueue.end()){
        // pack as many messages as possible
        if(channels[*it].size() + in_flight <= CWND){
            int32_t channel_id = *it;
            it = msgQueue.erase(it);
            send_message(channel_id);
        }else{
            it++;
            break;
        }
    }
    // Must have a packet in flight
    if(in_flight == 0){
        int32_t channel_id = msgQueue.front();
        msgQueue.pop_front();
        send_message(channel_id);
    }
    invariant_check3();
}


void MtpSrcOg::set_paths(vector<const Route*>* rt_list){
    _paths = *rt_list;
}

void MtpSrcOg::send_message(seq_t channel_id){
    set<MtpPacket*> message = channels[channel_id];

    // must receive ARX by this deadline
    deadlines[channel_id] = eventlist().now() + SHORT_FABRIC_RTO;
    eventlist().sourceIsPendingRel(*this, SHORT_FABRIC_RTO);
    in_flight += message.size();
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
    // cout << eventlist().now() << " ch: " << channel_id << " sz: " << message.size() << endl;
    // cout << "sending: " << message.size() << " ch: " << channel_id << endl;
    invariant_check3();
}

void MtpSrcOg::create_message(seq_t channel_id){
    assert(_route_strategy != MtpRouteStrategy::NOT_SET);
    channels[channel_id].clear();

    // generate message size
    mem_b ttl_pkts = w5distrib::sample();;
    int32_t msg_id = next_msg_id++;

    // cout << "created: " << ttl_pkts << " in ch:" << channel_id << endl;
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
void MtpSrcOg::free_message(seq_t channel_id){
    set<MtpPacket*> channel = channels[channel_id];
    for(MtpPacket* pkt: channel){
        pkt->free();
    }
}

void MtpSrcOg::receivePacket(Packet& pkt){
    // assert(invariant_check());
    // assert(invariant_check2());
    invariant_check3();
    // cout << min_CWND() << "cwnd " << in_flight << "if time us:" << timeAsUs(eventlist().now()) << endl;
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
                    in_flight+=channels[ack.get_channel_id()].size(); // undo the drop from earlier
                }else{
                    it++;
                }
            }

            // assert(invariant_check());
            // assert(channels[ack.get_channel_id()].size() <= in_flight);

            // cout << "ack: " << channels[ack.get_channel_id()].size() << " in ch:" << ack.get_channel_id() << endl;
            in_flight -= channels[ack.get_channel_id()].size();
            ecn_seen += ack.get_ecn();
            total_seen++;
            total_rcvd++;
            total_size+=channels[ack.get_channel_id()].size();

            free_message(ack.get_channel_id());
            create_message(ack.get_channel_id());
            msgQueue.push_back(ack.get_channel_id());
            // assert(invariant_check());

            // update CWND
            update_CWNDs();
            // cout << "updated-cwnd if:" << in_flight << " cwnd:" << min_CWND() << endl;

            // send based on CWND
            size_t CWND = min_CWND();

            auto it = msgQueue.begin();
            while(msgQueue.size() && in_flight < CWND && it != msgQueue.end()){
                // pack as many messages as possible
                if(channels[*it].size() + in_flight <= CWND){
                    int32_t channel_id = *it;
                    it = msgQueue.erase(it);
                    send_message(channel_id);
                }else{
                    it++;
                    break;
                }
            }
            // Must have a packet in flight
            if(in_flight == 0){
                int32_t channel_id = msgQueue.front();
                msgQueue.pop_front();
                send_message(channel_id);
            }
        
            ack.free();
            // assert(invariant_check());
            break;
        }
        case MTPARX:{
            MtpArx& arx = dynamic_cast<MtpArx&>(pkt);
            
            deadlines[arx.get_channel_id()] = eventlist().now() + LONG_PATHLET_RTO;
            eventlist().sourceIsPendingRel(*this, LONG_PATHLET_RTO);

            // cout << "arx: " << arx.get_channel_id() << " - " << deadlines[arx.get_channel_id()] << endl;
            ecn_seen += arx.get_ecn();
            total_seen++;

            // create a CWND
            if(!pathlets.count(arx.get_pathlet_id())){
                CWNDs[arx.get_pathlet_id()] = INIT_CWND;
                pathlets.insert(arx.get_pathlet_id());
            }

            const double feedback_ratio = (((double) arx.get_feedback())/256.0);
            const double round_ce_val = ((double) arx.get_msg_sz()) * feedback_ratio;
            round_ces[arx.get_pathlet_id()] += round_ce_val;
            round_total[arx.get_pathlet_id()] += arx.get_msg_sz();
            // cout << round_ce_val << " " << arx.get_msg_sz() << " " << feedback_ratio << endl;
            arx.free();
            
            // assert(invariant_check());
            break;
        }
        case MTPATX:{
            MtpAtx& atx = dynamic_cast<MtpAtx&>(pkt);

            // must receive ARX or ACK by this deadline
            deadlines[atx.get_channel_id()] = eventlist().now() + SHORT_FABRIC_RTO;
            eventlist().sourceIsPendingRel(*this, SHORT_FABRIC_RTO);

            // cout << "atx: " << atx.get_channel_id() << " - " << deadlines[atx.get_channel_id()] << endl;

            // copy size history into all packets in message, if new history
            if(atx.get_sizes().size() >= (*channels[atx.get_channel_id()].begin())->get_sizes().size()){
                set<MtpPacket*> msg  = channels[atx.get_channel_id()];
                for(MtpPacket* pkt: msg){
                    pkt->set_sizes(atx.get_sizes());
                }
            }

            atx.free();
            // assert(invariant_check());
            break;
        }
        default:
            break;
    }
    // assert(invariant_check2());
    // invariant_check3();
}

void MtpSrcOg::doNextEvent(){
    // assert(invariant_check());
    // assert(invariant_check2());
    // invariant_check3();
    // cout << min_CWND() << "cwnd " << in_flight << "if time us:" << timeAsUs(eventlist().now()) << endl;
    if(!_flow_started){
        _flow_started = true;
        startflow();
        // assert(invariant_check());
    }else{
        // check for timers -> then retransmit
        simtime_picosec now = eventlist().now();
        for(size_t channel = 0; channel < MAX_CHANNELS; channel++){
            if(deadlines[channel] <= now){
                // message expired
                _total_rtxs++;
                
                deadlines[channel] = UINT64_MAX;
                
                // assert(channels[channel].size() <= in_flight);

                in_flight -= channels[channel].size();

                // Retransmission
                if(in_flight + channels[channel].size() <= floor(min_CWND()) || in_flight == 0){
                    send_message(channel);
                }else{
                    msgQueue.push_front(channel);
                }
            }
        }
        // assert(invariant_check());
        // assert(invariant_check2());
        // invariant_check3();

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
            fprintf(senderlog, "%f,%d,%d,%d,%d,%d\n", timeAsMs(eventlist().now()), (int)min_CWND(), (int)_total_rtxs,  (int)txQueue.size(),(int)in_flight, (int)total_rcvd);
            nextWrite = eventlist().now() + timeFromUs(MTPOG_SENDERLOG_DELAY);
            eventlist().sourceIsPending(*this, nextWrite);
        }
    }
}



MtpSinkOg::MtpSinkOg(EventList& eventlist)
    : EventSource(eventlist, "mtp_sink"), _total_received(0), last_fully_rcvd_msg(MAX_CHANNELS), cur_msgs(MAX_CHANNELS), _ack_cooldowns(MAX_CHANNELS, 0), _used_channels(MAX_CHANNELS, false)
{
    _nodename = "MtpSinkOg";
}


void MtpSinkOg::receivePacket(Packet& pkt){
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
            if(mtppkt.get_ecn()){
                ack->set_ecn();
            }
        }else{
            sendAck = false; // cooldown check not passed
        }
    }else{
        cur_msgs[channel_id].insert(MtpPacket::cpypkt(&mtppkt));
        size_t message_pkts = mtppkt.get_ttl_pkts();
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
            if(mtppkt.get_ecn()){
                ack->set_ecn();
            }
            // cout << timeAsUs(eventlist().now()) << ": " << mtppkt.get_message_id() << "| "<< timeAsUs(eventlist().now() - mtppkt.ts()) << endl;
            _total_received++;
            // cout << _total_received << "| " << ack->get_message_id() << " " << timeAsUs(eventlist().now()) << endl;
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

void MtpSinkOg::connect(MtpSrcOg& src, Route& route){
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

void MtpSinkOg::set_paths(vector<const Route*>* rt_list){
    _paths = *rt_list;
}

// in_flight == sum(all channel msgs if channel not in msgQueue)