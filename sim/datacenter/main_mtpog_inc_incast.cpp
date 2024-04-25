
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
#define DEFAULT_QUEUE_SIZE 6667 // 6667
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
    double threshp = MsgQueue_THRESHOLD;
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
        }else if (!strcmp(argv[i],"-thresh")){
            threshp = atof(argv[i+1]);
            cout << "thresh "<< threshp << endl;
            i++;
        } else
	        exit_error(argv[0]);
        i++;
    }
    
    cout << "Logging to " << filename.str() << endl;
    //Logfile 
    Logfile logfile(filename.str(), eventlist);
    logfile.setStartTime(timeFromSec(0));

    // FIRST CONNECTION


    // create an MTP source
    MtpSrcOg* mtpSrcOg;
    // create an MTP sink
    MtpSinkOg* mtpSnk;

    // topology
    IncFatTreeTopology* top = new IncFatTreeTopology(no_of_nodes, queuesize, &logfile, 
					       &eventlist,NORMAL);

    // single src/dest
    int src = 0;
    int src2 = 1;
    int dest = 127;
    
    AbstractQueue* q1 = top->change_queue_s_downlink(127, nullptr, true, threshp, "mtpog_qsize.csv");
    vector<const Route*>* pathsTo1 = top->get_paths(src, dest);
    vector<const Route*>* pathsFrom1 = top->get_paths(dest, src);

    // single path
    const Route* routeTo1 = pathsTo1->at(0);
    const Route* routeFrom1 = pathsFrom1->at(0);
    Route routeToWithSink1 = *routeTo1;
    Route routeFromWithSrc1 = *routeFrom1;

    mtpSrcOg = new MtpSrcOg(NULL, NULL, eventlist, speedFromMbps((double)HOST_NIC), "mtpogsrc1.csv");
    mtpSnk = new MtpSinkOg(eventlist);
    routeToWithSink1.push_back(mtpSnk);
    routeFromWithSrc1.push_back(mtpSrcOg);

    routeToWithSink1.set_reverse(&routeFromWithSrc1);
    routeFromWithSrc1.set_reverse(&routeToWithSink1);

    double startTime = timeFromMs(0);
    mtpSrcOg->connect(routeToWithSink1, routeFromWithSrc1, *mtpSnk, startTime);

    // SECOND CONNECTION
    
    MtpSrcOg* mtpSrcOg2;
    MtpSinkOg* mtpSnk2;
    
    vector<const Route*>* pathsTo2 = top->get_paths(src2, dest);
    vector<const Route*>* pathsFrom2 = top->get_paths(dest, src2);

    // single path
    const Route* routeTo2 = pathsTo2->at(0);
    const Route* routeFrom2 = pathsFrom2->at(0);
    Route routeToWithSink2 = *routeTo2;
    Route routeFromWithSrc2 = *routeFrom2;

    mtpSrcOg2 = new MtpSrcOg(NULL, NULL, eventlist, speedFromMbps((double)HOST_NIC), "mtpogsrc2.csv");
    mtpSnk2 = new MtpSinkOg(eventlist);
    routeToWithSink2.push_back(mtpSnk2);
    routeFromWithSrc2.push_back(mtpSrcOg2);

    routeToWithSink2.set_reverse(&routeFromWithSrc2);
    routeFromWithSrc2.set_reverse(&routeToWithSink2);

    mtpSrcOg2->connect(routeToWithSink2, routeFromWithSrc2, *mtpSnk2, startTime);

    while(eventlist.doNextEvent());

    cout << "Done" << endl;
    cout << mtpSnk->get_total_received() + mtpSnk2->get_total_received() << endl;
    cout << mtpSrcOg->total_rtxs() + mtpSrcOg2->total_rtxs() << endl;
    cout << ((MsgQueue*) q1)->total_drops() << endl;
    cout << mtpSrcOg->avg_msg_size_rcvd() + mtpSrcOg2->avg_msg_size_rcvd() << endl;
}

string ntoa(double n) {
    stringstream s;
    s << n;
    return s.str();
}