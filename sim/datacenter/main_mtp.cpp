
#include <strstream>
#include <iostream>
#include <string.h>
#include "network.h"
#include "pipe.h"
#include "eventlist.h"
#include "clock.h"
#include "mtp.h"
#include "main.h"
#include "fat_tree_topology.h"

#define DEFAULT_NODES 128
#define DEFAULT_QUEUE_SIZE 8
#define END_TIME_SECS 0.201

/**
 * FAT TREE WITH MTP, NO INC ELEMENTS
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

    
    MtpSrc::setRouteStrategy(MtpRouteStrategy::SINGLE_PATH);
    MtpSink::setRouteStrategy(MtpRouteStrategy::SINGLE_PATH);

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
    MtpSrc* mtpSrc;
    // create an MTP sink
    MtpSink* mtpSnk;

    // topology
    FatTreeTopology* top = new FatTreeTopology(no_of_nodes, queuesize, &logfile, 
					       &eventlist,NULL,NORMAL,0);

    // single src/dest
    int src = 0;
    int dest = 127;
    vector<const Route*>* pathsTo = top->get_paths(src, dest);
    vector<const Route*>* pathsFrom = top->get_paths(dest, src);

    // single path
    const Route* routeTo = pathsTo->at(0);
    const Route* routeFrom = pathsFrom->at(0);
    Route routeToWithSink = *routeTo;
    Route routeFromWithSrc = *routeFrom;

    mtpSrc = new MtpSrc(NULL, NULL, eventlist);
    mtpSnk = new MtpSink(eventlist);
    routeToWithSink.push_back(mtpSnk);
    routeFromWithSrc.push_back(mtpSrc);

    double startTime = timeFromMs(1);
    mtpSrc->connect(routeToWithSink, routeFromWithSrc, *mtpSnk, startTime);

    while(eventlist.doNextEvent());

    cout << "Done" << endl;
    cout << mtpSnk->get_total_received() << endl;
}

string ntoa(double n) {
    stringstream s;
    s << n;
    return s.str();
}