#ifndef MTPPACKET_H
#define MTPPACKET_H

#include "network.h"
#include "unordered_map"

const static int ACK_SIZE = 64;

class MtpPacket : public Packet {
 public:
    typedef uint64_t seq_t;

   inline static MtpPacket* newpkt(PacketFlow& flow, seq_t pkt_seqno, mem_b ttl_pkts, int32_t message_id,
      int32_t src, int32_t dest, int32_t channel_no, uint16_t size, packetid_t id, unordered_map<size_t, mem_b> sizes){
      MtpPacket* p = _packetdb.allocPacket();
      p->_type = MTP;
      p->_size = size;
      p->_bounced = false;
      p->_is_header = false;
      p->_id = id;
      p->src = src;
      p->dest = dest;
      p->pkt_seqno = pkt_seqno;
      p->ttl_pkts = ttl_pkts;
      p->message_id = message_id;
      p->channel_id = channel_no;
      p->_flow = &flow;
      p->_nexthop = 0;
      p->sizes = sizes;
      p->ecn =false;
      return p;
   }

   inline static MtpPacket* cpypkt(MtpPacket* og){
      MtpPacket* p = _packetdb.allocPacket();
      p->_type = og->_type;
      p->_size = og->_size;
      p->_bounced = og->_bounced;
      p->_is_header = og->_is_header;
      p->_id = og->_id;
      p->src = og->src;
      p->dest = og->dest;
      p->pkt_seqno = og->pkt_seqno;
      p->ttl_pkts = og->ttl_pkts;
      p->message_id = og->message_id;
      p->channel_id = og->channel_id;
      p->_flow = og->_flow;
      p->_nexthop = og->_nexthop;
      p->sizes = og->sizes;
      p->set_route(*og->route());
      p->ecn=false;
      return p;
   }
  
    virtual inline void  strip_payload() {
	    Packet::strip_payload(); _size = ACK_SIZE;
    };
	
    void free() {_packetdb.freePacket(this);}
    virtual ~MtpPacket(){}
    inline simtime_picosec ts() const {return _ts;}
    inline void set_ts(simtime_picosec ts) {_ts = ts;}
    inline int32_t path_id() const {return _route->path_id();}
    inline int32_t get_src() const {return src;}
    inline int32_t get_dest() const {return dest;}
    inline int32_t get_message_id() const {return message_id;}
    inline mem_b get_ttl_pkts() const {return ttl_pkts;}
    inline int32_t get_channel_id() const {return channel_id;}
    inline seq_t get_pkt_seqno() const{return pkt_seqno;}
    inline unordered_map<size_t, mem_b> get_sizes() const {return sizes;}
    inline void set_sizes(unordered_map<size_t, mem_b> newSizes) {
      for(auto pair: newSizes){
         sizes[pair.first] = pair.second;
      }
    }
    inline void set_nexthop(uint32_t nexthop) {_nexthop = nexthop;}
    inline void set_ecn() {ecn = true;}
    inline bool get_ecn()const{return ecn;}
 protected:
    seq_t pkt_seqno;
    mem_b ttl_pkts;
    int32_t message_id;
    int32_t src;
    int32_t dest;
    int32_t channel_id;
    simtime_picosec _ts;
    static PacketDB<MtpPacket> _packetdb;
    unordered_map<size_t, mem_b> sizes;
    bool ecn;
};

inline bool operator<(const MtpPacket& lhs, const MtpPacket& rhs){
  return lhs.get_pkt_seqno() < rhs.get_pkt_seqno();
}

class MtpAck : public Packet {
 public:
   typedef MtpPacket::seq_t seq_t;

   inline static MtpAck* newpkt(PacketFlow& flow, int32_t message_id,
      int32_t src, int32_t dest, int32_t channel_no, simtime_picosec ts, packetid_t id){
      MtpAck* p = _packetdb.allocPacket();
      p->_type = MTPACK;
      p->_size = ACK_SIZE;
      p->_bounced = false;
      p->_is_header = true;
      p->_id = id;
      p->src = src;
      p->dest = dest;
      p->message_id = message_id;
      p->channel_id = channel_no;
      p->_ts = ts;
      p->_flow = &flow;
      p->_nexthop = 0;
      return p;
   }

   inline static MtpAck* cpypkt(MtpAck* og){
      MtpAck* p = _packetdb.allocPacket();
      p->_type = og->_type;
      p->_size = og->_size;
      p->_bounced = og->_bounced;
      p->_is_header = og->_is_header;
      p->_id = og->_id;
      p->src = og->src;
      p->dest = og->dest;
      p->message_id = og->message_id;
      p->channel_id = og->channel_id;
      p->_flow = og->_flow;
      p->_nexthop = og->_nexthop;
      p->set_route(*og->route());
      p->ecn = false;
      return p;
   }
  
    void free() {_packetdb.freePacket(this);}
    inline simtime_picosec ts() const {return _ts;}
    inline void set_ts(simtime_picosec ts) {_ts = ts;}
    inline int32_t get_channel_id() const {return channel_id;}
    inline int32_t get_message_id() const {return message_id;}
    inline void set_nexthop(uint32_t nexthop) {_nexthop = nexthop; }
    inline void set_ecn() {ecn = true;}
    inline bool get_ecn()const{return ecn;}
    virtual ~MtpAck(){}

 protected:
    seq_t message_id;
    int32_t channel_id;
    int32_t src;
    int32_t dest;
    simtime_picosec _ts;
    static PacketDB<MtpAck> _packetdb;
    bool ecn;
};



class MtpArx : public Packet{
   public:
   typedef MtpPacket::seq_t seq_t;
   inline static MtpArx* newpkt(PacketFlow& flow, int32_t message_id,
      int32_t pathlet_id, int32_t dest, int32_t channel_no, simtime_picosec ts, packetid_t id, bool slowdown,
      bool drop, uint8_t feedback, size_t msg_sz, bool tx_slowdown){
      MtpArx* p = _packetdb.allocPacket();
      p->_type = MTPARX;
      p->_size = ACK_SIZE;
      p->_bounced = false;
      p->_is_header = true;
      p->_id = id;
      p->pathlet_id = pathlet_id;
      p->dest = dest;
      p->message_id = message_id;
      p->channel_id = channel_no;
      p->slowdown = slowdown;
      p->_ts = ts;
      p->_flow = &flow;
      p->_nexthop = 0;
      p->drop = drop;
      p->feedback = feedback;
      p->msg_sz = msg_sz;
      p->tx_slowdown = tx_slowdown;
      p->ecn = false;
      return p;
   }

   void free() {_packetdb.freePacket(this);}
   inline simtime_picosec ts() const {return _ts;}
   inline void set_ts(simtime_picosec ts) {_ts = ts;}
   
   inline int32_t get_channel_id() const {return channel_id;}
   inline bool get_slowdown() const {return slowdown;}
   inline void set_nexthop(uint32_t nexthop) {_nexthop = nexthop; }
   inline int32_t get_message_id() const{return message_id;}
   inline bool dropped() const { return drop;}
   inline int32_t get_pathlet_id() const{return pathlet_id;}
   inline uint8_t get_feedback() const{return feedback;}
   inline size_t get_msg_sz() const{return msg_sz;}
   inline bool get_tx_slowdown() const { return tx_slowdown; }
   inline void set_ecn()  {ecn = true;}
   inline bool get_ecn() const {return ecn;}
   virtual ~MtpArx(){}

   protected:
      seq_t message_id;
      int32_t channel_id;
      int32_t src;
      int32_t dest;
      int32_t pathlet_id;
      bool slowdown;
      simtime_picosec _ts;
      static PacketDB<MtpArx> _packetdb;
      bool drop;
      uint8_t feedback;
      size_t msg_sz;
      bool tx_slowdown;
      bool ecn;
};

class MtpAtx : public Packet {
 public:
   typedef MtpPacket::seq_t seq_t;


   inline static MtpAtx* newpkt(PacketFlow& flow, int32_t message_id,
      int32_t pathlet_id, int32_t dest, int32_t channel_no, simtime_picosec ts, packetid_t id, bool slowdown,
      bool tx_drop, bool tx_slowdown){
      MtpAtx* p = _packetdb.allocPacket();
      p->_type = MTPATX;
      p->_size = ACK_SIZE;
      p->_bounced = false;
      p->_is_header = true;
      p->_id = id;
      p->pathlet_id = pathlet_id;
      p->dest = dest;
      p->message_id = message_id;
      p->channel_id = channel_no;
      p->_ts = ts;
      p->_flow = &flow;
      p->_nexthop = 0;
      p->slowdown = slowdown;
      p->tx_drop  = tx_drop;
      p->tx_slowdown = tx_slowdown;
      return p;
   }
  
    void free() {_packetdb.freePacket(this);}
    inline simtime_picosec ts() const {return _ts;}
    inline void set_ts(simtime_picosec ts) {_ts = ts;}
    inline int32_t get_channel_id() const {return channel_id;}
    inline void set_nexthop(uint32_t nexthop) {_nexthop = nexthop; }
    inline int32_t get_message_id() const{return message_id;}
    inline int32_t get_pathlet_id() const{return pathlet_id;}
    inline bool get_slowdown() const{return slowdown;}
    inline unordered_map<size_t, mem_b> get_sizes() const {return sizes;}
    inline void set_sizes(unordered_map<size_t, mem_b> newSizes) {
      for(auto pair: newSizes){
         sizes[pair.first] = pair.second;
      }
    }
    inline bool get_tx_slowdown() const { return tx_slowdown;}
    inline bool get_tx_dropped() const { return tx_drop;}
    virtual ~MtpAtx(){}

 protected:
    seq_t message_id;
    int32_t channel_id;
    int32_t src;
    int32_t dest;
    int32_t pathlet_id;
    simtime_picosec _ts;
    bool slowdown;
    static PacketDB<MtpAtx> _packetdb;
    unordered_map<size_t, mem_b> sizes;
    bool tx_drop;
    bool tx_slowdown;
};
#endif