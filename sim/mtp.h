#ifndef MTP_H
#define MTP_H

#include <list>
#include <set>
#include <unordered_map>
#include <random>
#include <map>
#include <cstdio>
#include "config.h"
#include "network.h"
#include "eventlist.h"
#include "mtppacket.h"
#include "w1distrib.h"
#include "w5distrib.h"

#define SIZE_EXP_LOWER 0
#define SIZE_EXP_UPPER 4.5
#define MAX_CHANNELS 256.0

class MtpSink;

enum class MtpRouteStrategy {NOT_SET, SINGLE_PATH, SCATTER_ROUNDROBIN, SCATTER_RANDOM};

// in picoseconds
// #define LONG_PATHLET_RTO 4e10 //4e9 // 20e12
// #define SHORT_FABRIC_RTO 75e9 // 28 ms ~ (drainTime * largest msg size, used to be 4e8)

#define ACK_COOLDOWN 1e8
#define OFFERED_LOAD 10000 // 10 Gbps
#define OFFER_INTERVAL 100 // microseconds

class MtpSrc : public PacketSink, public EventSource {
    friend class MtpSink;
        // in picoseconds
    static constexpr uint64_t LONG_PATHLET_RTO = 4e9;// 4e9 // 20e12
    static constexpr uint64_t SHORT_FABRIC_RTO = 75e9;
 public:
    typedef MtpPacket::seq_t seq_t;
    MtpSrc(MtpLogger* logger, TrafficLogger* pktlogger, EventList &eventlist, linkspeed_bps bitrate, char* lfile,
    bool immediate, uint64_t offered_load);
    uint32_t get_id(){ return id;}
    virtual void connect(Route& routeout, Route& routeback, MtpSink& sink, simtime_picosec startTime);
    void startflow();
    virtual void doNextEvent();
    virtual void receivePacket(Packet& pkt);
    void set_paths(vector<const Route*>* rt);
    static void setRouteStrategy(MtpRouteStrategy strat){_route_strategy = strat;}
    
    // Required by DataReceiver
    virtual const string& nodename() { return _nodename; }
    
    inline size_t total_rtxs() const {return _total_rtxs;}
    inline double avg_msg_size_rcvd() const {
      return total_size/total_rcvd;
    }

    inline simtime_picosec drainTime(Packet *pkt) { 
      return (simtime_picosec)(pkt->size() * _ps_per_byte); 
    }

    static MtpRouteStrategy _route_strategy;
    
 private:

   // for channels
   void free_message(seq_t channel_id);
   void send_message(seq_t channel_id);
   void create_message(seq_t channel_id);
   // void offer_load();
   bool invariant_check();

    list<set<MtpPacket*>> load;
    // // generator
    uint64_t _offered_load; // packets per second for offered_load

    list<MtpPacket*> txQueue;

    // Housekeeping
    MtpLogger* _logger;
    TrafficLogger* _pktlogger;

    // Connectivity
    PacketFlow _flow;
    string _nodename;
    MtpSink* _sink;

    // single path route
    Route* _route;

    // keeping track of messages
    vector<set<MtpPacket*>> channels;
    list<int32_t> msgQueue;
    simtime_picosec nextDrain;
    simtime_picosec _ps_per_byte;
    linkspeed_bps _bitrate;
    
    // inflight messages
    double CWND;
    size_t in_flight;

    // clogged pathlets
    set<int32_t> filling_pathlets;
    
    // retransmit expired channels
    vector<simtime_picosec> deadlines;
    
    
    size_t next_msg_id;
    size_t _total_rtxs = 0;

    double total_rcvd = 0;
    double total_size = 0;

    simtime_picosec nextWrite; 
    FILE *senderlog;
    size_t rr_ctr;
    bool _flow_started;
    vector<const Route*> _paths;
    simtime_picosec _start;

    linkspeed_bps _offered_load;
    bool im;
    vector<set<MtpPacket*>> load;
    
};

class MtpSink : public PacketSink, public DataReceiver, public EventSource {
    friend class MtpSrc;
 public:
    typedef MtpPacket::seq_t seq_t;
    MtpSink(EventList& ev);
 
    uint32_t get_id(){ return id;}
    void receivePacket(Packet& pkt);
    
    MtpSrc* _src;

    // Connectivity
    void connect(MtpSrc& src, Route& route);
    void set_paths(vector<const Route*>* rt);

    inline uint64_t get_total_received(){
      return _total_received;
    }
    static void setRouteStrategy(MtpRouteStrategy strat) {_route_strategy = strat;}
    
    // Required by DataReceiver
    virtual const string& nodename() { return _nodename; }
    virtual uint64_t cumulative_ack(){return 0;}
    virtual uint32_t drops(){return 0;}
    virtual void doNextEvent(){}
 private:
    string _nodename;

    uint64_t _total_received;
    vector<MtpAck*> last_fully_rcvd_msg;
    vector<set<MtpPacket*>> cur_msgs;
    
    static MtpRouteStrategy _route_strategy;
    vector<const Route*> _paths;
    Route* _route; //single path
    size_t rr_ctr;
    
    vector<simtime_picosec> _ack_cooldowns; // last time sent
    vector<bool> _used_channels;
};


#endif