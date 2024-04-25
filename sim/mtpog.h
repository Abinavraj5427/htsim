#ifndef MTPOG_H
#define MTPOG_H

#include <list>
#include <set>
#include <unordered_map>
#include <random>
#include <map>
#include "config.h"
#include "network.h"
#include "eventlist.h"
#include "mtppacket.h"
#include "w5distrib.h"

#define SIZE_EXP_LOWER 0
#define SIZE_EXP_UPPER 4.5
#define MAX_CHANNELS 256.0

class MtpSinkOg;

enum class MtpRouteStrategy {NOT_SET, SINGLE_PATH, SCATTER_ROUNDROBIN, SCATTER_RANDOM};



#define ACK_COOLDOWN 1e8

class MtpSrcOg : public PacketSink, public EventSource {
    friend class MtpSinkOg;
    // in picoseconds
    static constexpr uint64_t LONG_PATHLET_RTO = 4e9;// 4e9 // 20e12
    static constexpr uint64_t SHORT_FABRIC_RTO = 75e9;
 public:
    typedef MtpPacket::seq_t seq_t;
    MtpSrcOg(MtpLogger* logger, TrafficLogger* pktlogger, EventList &eventlist, linkspeed_bps bitrate, string fp);
    uint32_t get_id(){ return id;}
    virtual void connect(Route& routeout, Route& routeback, MtpSinkOg& sink, simtime_picosec startTime);
    void startflow();
    virtual void doNextEvent();
    virtual void receivePacket(Packet& pkt);
    void set_paths(vector<const Route*>* rt);
    static void setRouteStrategy(MtpRouteStrategy strat){_route_strategy = strat;}
    
    // Required by DataReceiver
    virtual const string& nodename() { return _nodename; }

    static MtpRouteStrategy _route_strategy;
    inline size_t total_rtxs() const {return _total_rtxs;}
    inline double avg_msg_size_rcvd() const {return total_size/total_rcvd;}
    
    inline simtime_picosec drainTime(Packet *pkt) { 
      return (simtime_picosec)(pkt->size() * _ps_per_byte); 
    }

    
 private:

   // for channels
   void free_message(seq_t channel_id);
   void send_message(seq_t channel_id);
   void create_message(seq_t channel_id);

   bool invariant_check();
   bool invariant_check2();
   void invariant_check3();

   double min_CWND();
   void update_CWNDs();

    // Housekeeping
    MtpLogger* _logger;
    TrafficLogger* _pktlogger;

    // Connectivity
    PacketFlow _flow;
    string _nodename;
    MtpSinkOg* _sink;

    // single path route
    Route* _route;
    vector<const Route*> _paths;

    // keeping track of messages
    vector<set<MtpPacket*>> channels;
    list<int32_t> msgQueue;
    list<MtpPacket*> txQueue;
    simtime_picosec nextDrain;
    simtime_picosec _ps_per_byte;
    linkspeed_bps _bitrate;

    size_t in_flight = 0;

    double ecn_seen = 0;
    double total_seen = 0;
    double alpha = 0;
    double g = 0.8;
    double link_CWND = 3000;

    unordered_map<size_t, double> CWNDs;
    unordered_map<size_t, double> round_ces;
    unordered_map<size_t, double> round_total;
    set<int32_t> pathlets;
    
    // inflight messages
    // size_t CWND;
    size_t CWND_incr_ctr;

    // clogged pathlets
    set<int32_t> filling_pathlets;
    
    // retransmit expired channels
    vector<simtime_picosec> deadlines;

    size_t next_msg_id;
    size_t rr_ctr;
    size_t _total_rtxs = 0;

    double total_rcvd = 0;
    double total_size = 0;
    bool _flow_started;

    simtime_picosec nextWrite;
    FILE *senderlog;
};

class MtpSinkOg : public PacketSink, public DataReceiver, public EventSource {
    friend class MtpSrcOg;
 public:
    typedef MtpPacket::seq_t seq_t;
    MtpSinkOg(EventList& ev);
 
    uint32_t get_id(){ return id;}
    void receivePacket(Packet& pkt);
    
    MtpSrcOg* _src;

    // Connectivity
    void connect(MtpSrcOg& src, Route& route);
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