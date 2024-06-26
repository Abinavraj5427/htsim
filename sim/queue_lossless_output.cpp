// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-        
#include <math.h>
#include <iostream>
#include <sstream>
#include "switch.h"
#include "queue_lossless_output.h"

LosslessOutputQueue::LosslessOutputQueue(linkspeed_bps bitrate, mem_b maxsize, 
					 EventList& eventlist, QueueLogger* logger, int ECN, int K)
    : Queue(bitrate,maxsize,eventlist,logger), 
      _state_send(READY)
{
    //assume worst case: PAUSE frame waits for one MSS packet to be sent to other switch, and there is 
    //an MSS just beginning to be sent when PAUSE frame arrives; this means 2 packets per incoming
    //port, and we must have buffering for all ports except this one (assuming no one hop cycles!)

    _sending = 0;

    _ecn_enabled = ECN;
    _K = K;

    stringstream ss;
    ss << "queue lossless output(" << bitrate/1000000 << "Mb/s," << maxsize << "bytes)";
    _nodename = ss.str();
}


void
LosslessOutputQueue::receivePacket(Packet& pkt){
    receivePacket(pkt,NULL);
}

void
LosslessOutputQueue::receivePacket(Packet& pkt,VirtualQueue* prev) 
{
    //is this a PAUSE frame? 
    if (pkt.type()==ETH_PAUSE){
	EthPausePacket* p = (EthPausePacket*)&pkt;

	if (p->sleepTime()>0){
	    //remote end is telling us to shut up.
	    //assert(_state_send == READY);
	    if (_sending)
		//we have a packet in flight
		_state_send = PAUSE_RECEIVED;
	    else
		_state_send = PAUSED;

	    //cout << timeAsMs(eventlist().now()) << " " << _name << " PAUSED "<<endl;	    
	}
	else {
	    //we are allowed to send!
	    _state_send = READY;
	    //cout << timeAsMs(eventlist().now()) << " " << _name << " GO "<<endl;

	    //start transmission if we have packets to send!
	    if(_enqueued.size()>0&&!_sending)
		beginService();
	}
	
	pkt.free();
	return;
    }

    /* normal packet, enqueue it */

    //remember the virtual queue that has sent us this packet; will notify the vq once the packet has left our buffer.
    assert(prev!=NULL);

    pkt.flow().logTraffic(pkt, *this, TrafficLogger::PKT_ARRIVE);

    bool queueWasEmpty = _enqueued.empty();

    _vq.push_front(prev);
    _enqueued.push_front(&pkt);


    _queuesize += pkt.size();

    if (_queuesize > _maxsize){
	cout << " Queue " << _name << " LOSSLESS not working! I should have dropped this packet" << endl;
    }

    if (_logger) 
	((QueueLogger*) _logger)->logQueue(*this, QueueLogger::PKT_ENQUEUE, pkt);

    if (queueWasEmpty && _state_send == READY) {
	/* schedule the dequeue event */
	assert(_enqueued.size()==1);
	beginService();
    }
}

void LosslessOutputQueue::beginService(){
    assert(_state_send==READY&&!_sending);
    Queue::beginService();
    _sending = 1;
}

void LosslessOutputQueue::completeService(){
    /* dequeue the packet */
    assert(!_enqueued.empty());
    Packet* pkt = _enqueued.back();
    VirtualQueue* q = _vq.back();

    _enqueued.pop_back();
    _vq.pop_back();

    //mark on deque
    if (_ecn_enabled && _queuesize > _K)
	  pkt->set_flags(pkt->flags() | ECN_CE);    

    _queuesize -= pkt->size();

    pkt->flow().logTraffic(*pkt, *this, TrafficLogger::PKT_DEPART);
    if (_logger) ((QueueLogger*) _logger)->logQueue(*this, QueueLogger::PKT_SERVICE, *pkt);

    //tell the virtual input queue this packet is done!
    q->completedService(*pkt);

    /* tell the packet to move on to the next pipe */
    pkt->sendOn();

    _sending = 0;

    //if (_state_send!=READY){
    //cout << timeAsMs(eventlist().now()) << " queue " << _name << " not ready but sending pkt " << pkt->type() << endl;
    //}

    if (_state_send == PAUSE_RECEIVED)
	_state_send = PAUSED;

    if (!_enqueued.empty()) {
	if (_state_send == READY)
	    /* start packet transmission, schedule the next dequeue event */
	    beginService();
    }
}
