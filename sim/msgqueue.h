#ifndef MTP_QUEUE_H
#define MTP_QUEUE_H


/**
 * A mtp queue that maintains a threshold to signal congestion
 * while storing messages.
*/
#include "w1distrib.h"
#include "w5distrib.h"
#include "eventlist.h"
#include "loggertypes.h"
#include "queue.h"
#include "mtppacket.h"
#include <unordered_map>
#include <set>
#include <random>
#include <cstdio>

#define RESIZE_EXP_LOWER 0
#define RESIZE_EXP_UPPER 4.5
#define MsgQueue_THRESHOLD 0.5
#define MTP_QSIZE_DELAY (uint32_t) 20 //microseconds
struct MtpMsgStore{
    set<MtpPacket*> packets;
    simtime_picosec deadline; // 100 microsecs deadline
};

class MsgQueue: public AbstractQueue{
    static constexpr uint64_t PROCESSING_TIME_MEAN = 50; // 18000 // microseconds
    public:
        MsgQueue(linkspeed_bps bitrate, mem_b maxsize,
            EventList &eventlist, AbstractQueueLogger* logger, double thresholdp, std::string fp);
        ~MsgQueue(){
            fclose(logsize);
        }
        virtual void receivePacket(Packet& pkt);
        virtual void doNextEvent();

        // logger functions
        inline mem_b queuesize(){
            return ttl_pkts;
        }

        inline uint32_t total_drops(){
            return _ttl_drops;
        }

        inline uint32_t total_expirations(){
            return _ttl_expirations;
        }

    protected:
        
        std::random_device rd;
        std::poisson_distribution<uint32_t> processingDistribution;
        simtime_picosec procEndTime;

        list<set<MtpPacket*>> readyq;
        unordered_map<uint64_t, MtpMsgStore> msgStore;
        list<MtpPacket*> txQueue;

        simtime_picosec nextDrainTime = UINT64_MAX;
        mem_b ttl_pkts = 0;
        uint32_t threshold_size;
        uint32_t _maxpkts;

        // For loggers
        uint32_t _ttl_drops = 0;
        uint32_t _ttl_expirations = 0;
        simtime_picosec nextWrite;

        FILE *logsize;

};


#endif