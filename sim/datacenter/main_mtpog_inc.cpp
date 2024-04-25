
#include <strstream>
#include <iostream>
#include <string.h>
#include "network.h"
#include "pipe.h"
#include "eventlist.h"
#include "clock.h"
#include "mtpog.h"
#include "main.h"
#include "inc_fat_tree_topology.h"

#define DEFAULT_NODES 128
#define DEFAULT_QUEUE_SIZE 6667
#define END_TIME_SECS 1 // .001

/**
 * FAT TREE WITH MTP INC ELEMENTS
*/

uint32_t RTT = 1; // microseconds, used for queues delay

EventList eventlist;

void exit_error(char* progr) {
    cout << "Usage " << progr << " not a valid argument" << endl;
    exit(1);
}

int main(int argc, char** argv){

    /*
    Set packet size to 9000 temporarily
    TODO: see if we can do variable size
    */
    Packet::set_packet_size(9000);

    /*
    Change later since this ends simulation.
    Default: no time limit end.
    */
    eventlist.setEndtime(timeFromSec(END_TIME_SECS));

    
    MtpSrcOg::setRouteStrategy(MtpRouteStrategy::SINGLE_PATH);
    MtpSinkOg::setRouteStrategy(MtpRouteStrategy::SINGLE_PATH);
    w5distrib::fill_distrib();

    //Clock c(timeFromSec(5/100), eventlist); // TODO: Do I need this?
    
    mem_b queuesize = memFromPkt(DEFAULT_QUEUE_SIZE);
    int no_of_conns = 0, cwnd = 15, no_of_nodes = DEFAULT_NODES;
    stringstream filename(ios_base::out);

    filename << "logout.dat";
    int i = 1;
    while(i < argc){
        if(!strcmp(argv[i], "-o")){
            filename.str(std::string());
            filename << argv[i+1];
            i++;
        } else if (!strcmp(argv[i], "-conns")){
            no_of_conns = atoi(argv[i+1]);
            cout << "no_of_conns "<<no_of_conns << endl;
            i++;
        } else if (!strcmp(argv[i],"-nodes")){
            no_of_nodes = atoi(argv[i+1]);
            cout << "no_of_nodes "<<no_of_nodes << endl;
            i++;
        } else if (!strcmp(argv[i],"-cwnd")){
            cwnd = atoi(argv[i+1]);
            cout << "cwnd "<< cwnd << endl;
            i++;
        } else
	        exit_error(argv[0]);
        i++;
    }
    
    cout << "Logging to " << filename.str() << endl;
    //Logfile 
    Logfile logfile(filename.str(), eventlist);
    logfile.setStartTime(timeFromSec(0));

    // create an MTP source
    MtpSrcOg* mtpsrc;
    // create an MTP sink
    MtpSinkOg* mtpSnk;

    // topology
    IncFatTreeTopology* top = new IncFatTreeTopology(no_of_nodes, queuesize, &logfile, 
					       &eventlist,NORMAL);

    // single src/dest
    int src = 0;
    int dest = 127;
    
    AbstractQueue* q1 = top->change_queue_s_uplink(0, nullptr, true, 0, "mtpog_qsize.csv");
    vector<const Route*>* pathsTo = top->get_paths(src, dest);
    vector<const Route*>* pathsFrom = top->get_paths(dest, src);

    // single path
    const Route* routeTo = pathsTo->at(0);
    const Route* routeFrom = pathsFrom->at(0);
    Route routeToWithSink = *routeTo;
    Route routeFromWithSrc = *routeFrom;
    
    mtpsrc = new MtpSrcOg(NULL, NULL, eventlist, timeFromUs((double)HOST_NIC), "mtpogsrc.csv");
    mtpSnk = new MtpSinkOg(eventlist);
    routeToWithSink.push_back(mtpSnk);
    routeFromWithSrc.push_back(mtpsrc);

    routeToWithSink.set_reverse(&routeFromWithSrc);
    routeFromWithSrc.set_reverse(&routeToWithSink);

    double startTime = timeFromMs(0);
    mtpsrc->connect(routeToWithSink, routeFromWithSrc, *mtpSnk, startTime);

    while(eventlist.doNextEvent());

    cout << "Done" << endl;
    cout << mtpSnk->get_total_received() << endl;
    cout << mtpsrc->total_rtxs() << endl;
    cout << ((MsgQueueOg*) q1)->total_drops() << endl;
    cout << mtpsrc->avg_msg_size_rcvd() << endl;
}

string ntoa(double n) {
    stringstream s;
    s << n;
    return s.str();
}