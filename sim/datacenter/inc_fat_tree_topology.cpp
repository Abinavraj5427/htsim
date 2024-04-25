#include "inc_fat_tree_topology.h"
#include "queue.h"
#include "switch.h"
#include "compositequeue.h"
#include "prioqueue.h"
#include "queue_lossless.h"
#include "queue_lossless_input.h"
#include "queue_lossless_output.h"
#include "ecnqueue.h"
#include "randomqueue.h"

extern uint32_t RTT;
string ntoa(double n);

IncFatTreeTopology::IncFatTreeTopology(int no_of_nodes, mem_b queuesize, Logfile* lg,
        EventList* ev, queue_type q){
    _queuesize = queuesize;
    logfile = lg;
    qt = q;
    eventlist = ev;
    set_params(no_of_nodes);
    init_network();
}

void IncFatTreeTopology::set_params(int no_of_nodes) {

    // finding the right K size for fat tree
    _no_of_nodes = 0;
    K = 0;
    while(_no_of_nodes < no_of_nodes) {
        K++;
        _no_of_nodes = K * K * K /4;
    }

    // size overflow case
    if(_no_of_nodes > no_of_nodes){
        cerr << "Topology Error: can't have a FatTree with " << no_of_nodes
	     << " nodes\n";
	    exit(1);
    }

    NK = (K*K/2);
    NC = (K*K/4);
    NSRV = (K*K*K/4);
    cout << "_no_of_nodes " << _no_of_nodes << endl;
    cout << "K " << K << endl;
    cout << "Queue type " << qt << endl;

    // resizing queues, pipes, and switches
    switches_lp.resize(NK,NULL);
    switches_up.resize(NK,NULL);
    switches_c.resize(NC,NULL);

    pipes_nc_nup.resize(NC, vector<Pipe*>(NK));
    pipes_nup_nlp.resize(NK, vector<Pipe*>(NK));
    pipes_nlp_ns.resize(NK, vector<Pipe*>(NSRV));
    queues_nc_nup.resize(NC, vector<AbstractQueue*>(NK));
    queues_nup_nlp.resize(NK, vector<AbstractQueue*>(NK));
    queues_nlp_ns.resize(NK, vector<AbstractQueue*>(NSRV));

    pipes_nup_nc.resize(NK, vector<Pipe*>(NC));
    pipes_nlp_nup.resize(NK, vector<Pipe*>(NK));
    pipes_ns_nlp.resize(NSRV, vector<Pipe*>(NK));
    queues_nup_nc.resize(NK, vector<AbstractQueue*>(NC));
    queues_nlp_nup.resize(NK, vector<AbstractQueue*>(NK));
    queues_ns_nlp.resize(NSRV, vector<AbstractQueue*>(NK));
}

void check_not_null(Route* rt){
    int fail = 0;
    for (unsigned int i=1;i<rt->size()-1;i+=2){
        if (rt->at(i)==NULL){
            fail = 1;
            break;
        }
    }
    if (fail){
        for (unsigned int i=1;i<rt->size()-1;i+=2){
            printf("%p ",rt->at(i));
        }
        cout<<endl;
        assert(0);
    
    }
}

AbstractQueue* IncFatTreeTopology::alloc_src_queue(QueueLogger* queueLogger){
    return  new PriorityQueue(speedFromMbps((uint64_t)HOST_NIC), memFromPkt(FEEDER_BUFFER), *eventlist, queueLogger);
}

AbstractQueue* IncFatTreeTopology::alloc_queue(QueueLogger* queueLogger, mem_b queuesize){
    return alloc_queue(queueLogger, HOST_NIC, queuesize);
}

AbstractQueue* IncFatTreeTopology::alloc_queue(QueueLogger* queueLogger, uint64_t speed, mem_b queuesize){
    if (qt==RANDOM)
	    return new RandomQueue(speedFromMbps(speed), memFromPkt(SWITCH_BUFFER + RANDOM_BUFFER), *eventlist, queueLogger, memFromPkt(RANDOM_BUFFER));
    else if (qt==COMPOSITE)
	    return new CompositeQueue(speedFromMbps(speed), queuesize, *eventlist, queueLogger);
    else if (qt==CTRL_PRIO)
	    return new CtrlPrioQueue(speedFromMbps(speed), queuesize, *eventlist, queueLogger);
    else if (qt==ECN)
	    return new ECNQueue(speedFromMbps(speed), memFromPkt(2*SWITCH_BUFFER), *eventlist, queueLogger, memFromPkt(15));
    else if (qt==LOSSLESS)
	    return new LosslessQueue(speedFromMbps(speed), memFromPkt(50), *eventlist, queueLogger, NULL);
    else if (qt==LOSSLESS_INPUT)
	    return new LosslessOutputQueue(speedFromMbps(speed), memFromPkt(200), *eventlist, queueLogger);    
    else if (qt==LOSSLESS_INPUT_ECN)
	    return new LosslessOutputQueue(speedFromMbps(speed), memFromPkt(10000), *eventlist, queueLogger,1,memFromPkt(16));
    else if (qt==NORMAL)
        return new Queue(speedFromMbps(speed), queuesize, *eventlist, queueLogger);
    assert(0);
}

void IncFatTreeTopology::init_network(){
    for (int j=0;j<NC;j++)
        for (int k=0;k<NK;k++){
            queues_nc_nup[j][k] = NULL;
            pipes_nc_nup[j][k] = NULL;
            queues_nup_nc[k][j] = NULL;
            pipes_nup_nc[k][j] = NULL;
        }
    
    for (int j=0;j<NK;j++)
        for (int k=0;k<NK;k++){
            queues_nup_nlp[j][k] = NULL;
            pipes_nup_nlp[j][k] = NULL;
            queues_nlp_nup[k][j] = NULL;
            pipes_nlp_nup[k][j] = NULL;
        }

    for (int j=0;j<NK;j++)
        for (int k=0;k<NSRV;k++){
            queues_nlp_ns[j][k] = NULL;
            pipes_nlp_ns[j][k] = NULL;
            queues_ns_nlp[k][j] = NULL;
            pipes_ns_nlp[k][j] = NULL;
        }

    // TODO: Replace logger with different queue logger
    QueueLogger* queueLogger = nullptr;

    // links from lower layer pod switch to server
    for (int j = 0; j < NK; j++) {
        for (int l = 0; l < K/2; l++) {
            int k = j * K/2 + l;
            // Downlink
            queues_nlp_ns[j][k] = alloc_queue(queueLogger, _queuesize);
            queues_nlp_ns[j][k]->setName("LS" + ntoa(j) + "->DST" +ntoa(k));
            logfile->writeName(*(queues_nlp_ns[j][k]));

            pipes_nlp_ns[j][k] = new Pipe(timeFromUs(RTT), *eventlist);
            pipes_nlp_ns[j][k]->setName("Pipe-LS" + ntoa(j)  + "->DST" + ntoa(k));
            logfile->writeName(*(pipes_nlp_ns[j][k]));
            
            // Uplink
            queues_ns_nlp[k][j] = alloc_src_queue(queueLogger);
            queues_ns_nlp[k][j]->setName("SRC" + ntoa(k) + "->LS" +ntoa(j));
            logfile->writeName(*(queues_ns_nlp[k][j]));

            pipes_ns_nlp[k][j] = new Pipe(timeFromUs(RTT), *eventlist);
            pipes_ns_nlp[k][j]->setName("Pipe-SRC" + ntoa(k) + "->LS" + ntoa(j));
            logfile->writeName(*(pipes_ns_nlp[k][j]));
        }
    }

    for (int j = 0; j < NK; j++) {
        int podid = 2*j/K;
        //Connect the lower layer switch to the upper layer switches in the same pod
        for (int k=MIN_POD_ID(podid); k<=MAX_POD_ID(podid);k++){
            // Downlink
            queues_nup_nlp[k][j] = alloc_queue(queueLogger, _queuesize);
            queues_nup_nlp[k][j]->setName("US" + ntoa(k) + "->LS_" + ntoa(j));
            logfile->writeName(*(queues_nup_nlp[k][j]));
            
            pipes_nup_nlp[k][j] = new Pipe(timeFromUs(RTT), *eventlist);
            pipes_nup_nlp[k][j]->setName("Pipe-US" + ntoa(k) + "->LS" + ntoa(j));
            logfile->writeName(*(pipes_nup_nlp[k][j]));
            
            // Uplink
            queues_nlp_nup[j][k] = alloc_queue(queueLogger, _queuesize);
            queues_nlp_nup[j][k]->setName("LS" + ntoa(j) + "->US" + ntoa(k));
            logfile->writeName(*(queues_nlp_nup[j][k]));

            pipes_nlp_nup[j][k] = new Pipe(timeFromUs(RTT), *eventlist);
            pipes_nlp_nup[j][k]->setName("Pipe-LS" + ntoa(j) + "->US" + ntoa(k));
            logfile->writeName(*(pipes_nlp_nup[j][k]));
        }
    }

     // Upper layer in pod to core!
    for (int j = 0; j < NK; j++) {
        int podpos = j%(K/2);
        for (int l = 0; l < K/2; l++) {
            int k = podpos * K/2 + l;
            // Downlink
            queues_nup_nc[j][k] = alloc_queue(queueLogger, _queuesize);
            queues_nup_nc[j][k]->setName("US" + ntoa(j) + "->CS" + ntoa(k));
            logfile->writeName(*(queues_nup_nc[j][k]));
            
            pipes_nup_nc[j][k] = new Pipe(timeFromUs(RTT), *eventlist);
            pipes_nup_nc[j][k]->setName("Pipe-US" + ntoa(j) + "->CS" + ntoa(k));
            logfile->writeName(*(pipes_nup_nc[j][k]));
            
            queues_nc_nup[k][j] = alloc_queue(queueLogger, _queuesize);
            queues_nc_nup[k][j]->setName("CS" + ntoa(k) + "->US" + ntoa(j));
            logfile->writeName(*(queues_nc_nup[k][j]));

            pipes_nc_nup[k][j] = new Pipe(timeFromUs(RTT), *eventlist);
            pipes_nc_nup[k][j]->setName("Pipe-CS" + ntoa(k) + "->US" + ntoa(j));
            logfile->writeName(*(pipes_nc_nup[k][j]));
        }
    }
}

vector<const Route*>* IncFatTreeTopology::get_paths(int src, int dest){
    vector<const Route*>* paths = new vector<const Route*>();
    route_t *routeout, *routeback;
    if (HOST_POD_SWITCH(src)==HOST_POD_SWITCH(dest)){
        // forward path
        routeout = new Route();

        routeout->push_back(queues_ns_nlp[src][HOST_POD_SWITCH(src)]);
        routeout->push_back(pipes_ns_nlp[src][HOST_POD_SWITCH(src)]);

        routeout->push_back(queues_nlp_ns[HOST_POD_SWITCH(dest)][dest]);
        routeout->push_back(pipes_nlp_ns[HOST_POD_SWITCH(dest)][dest]);

        // reverse path for RTS packets
        routeback = new Route();
        routeback->push_back(queues_ns_nlp[dest][HOST_POD_SWITCH(dest)]);
        routeback->push_back(pipes_ns_nlp[dest][HOST_POD_SWITCH(dest)]);

        routeback->push_back(queues_nlp_ns[HOST_POD_SWITCH(src)][src]);
        routeback->push_back(pipes_nlp_ns[HOST_POD_SWITCH(src)][src]);

        routeout->set_reverse(routeback);
        routeback->set_reverse(routeout);

        paths->push_back(routeout);

        return paths;
    }
    else if (HOST_POD(src)==HOST_POD(dest)){
        //don't go up the hierarchy, stay in the pod only.

        int pod = HOST_POD(src);
        //there are K/2 paths between the source and the destination
        for (int upper = MIN_POD_ID(pod);upper <= MAX_POD_ID(pod); upper++){
            //upper is nup
            routeout = new Route();

            routeout->push_back(queues_ns_nlp[src][HOST_POD_SWITCH(src)]);
            routeout->push_back(pipes_ns_nlp[src][HOST_POD_SWITCH(src)]);

            routeout->push_back(queues_nlp_nup[HOST_POD_SWITCH(src)][upper]);
            routeout->push_back(pipes_nlp_nup[HOST_POD_SWITCH(src)][upper]);

            routeout->push_back(queues_nup_nlp[upper][HOST_POD_SWITCH(dest)]);
            routeout->push_back(pipes_nup_nlp[upper][HOST_POD_SWITCH(dest)]);

            routeout->push_back(queues_nlp_ns[HOST_POD_SWITCH(dest)][dest]);
            routeout->push_back(pipes_nlp_ns[HOST_POD_SWITCH(dest)][dest]);

            // reverse path for RTS packets
            routeback = new Route();
            
            routeback->push_back(queues_ns_nlp[dest][HOST_POD_SWITCH(dest)]);
            routeback->push_back(pipes_ns_nlp[dest][HOST_POD_SWITCH(dest)]);

            routeback->push_back(queues_nlp_nup[HOST_POD_SWITCH(dest)][upper]);
            routeback->push_back(pipes_nlp_nup[HOST_POD_SWITCH(dest)][upper]);

            routeback->push_back(queues_nup_nlp[upper][HOST_POD_SWITCH(src)]);
            routeback->push_back(pipes_nup_nlp[upper][HOST_POD_SWITCH(src)]);

            routeback->push_back(queues_nlp_ns[HOST_POD_SWITCH(src)][src]);
            routeback->push_back(pipes_nlp_ns[HOST_POD_SWITCH(src)][src]);

            routeout->set_reverse(routeback);
            routeback->set_reverse(routeout);

            paths->push_back(routeout);
        }
        return paths;
    } else {
        int pod = HOST_POD(src);

        for (int upper = MIN_POD_ID(pod);upper <= MAX_POD_ID(pod); upper++){
            for (int core = (upper%(K/2)) * K / 2; core < ((upper % (K/2)) + 1)*K/2; core++){
                //upper is nup
                routeout = new Route();
	
                routeout->push_back(queues_ns_nlp[src][HOST_POD_SWITCH(src)]);
                routeout->push_back(pipes_ns_nlp[src][HOST_POD_SWITCH(src)]);

                routeout->push_back(queues_nlp_nup[HOST_POD_SWITCH(src)][upper]);
                routeout->push_back(pipes_nlp_nup[HOST_POD_SWITCH(src)][upper]);

                routeout->push_back(queues_nup_nc[upper][core]);
                routeout->push_back(pipes_nup_nc[upper][core]);

	            //now take the only link down to the destination server!
	
                int upper2 = HOST_POD(dest)*K/2 + 2 * core / K;
                routeout->push_back(queues_nc_nup[core][upper2]);
                routeout->push_back(pipes_nc_nup[core][upper2]);

                routeout->push_back(queues_nup_nlp[upper2][HOST_POD_SWITCH(dest)]);
                routeout->push_back(pipes_nup_nlp[upper2][HOST_POD_SWITCH(dest)]);

                routeout->push_back(queues_nlp_ns[HOST_POD_SWITCH(dest)][dest]);
                routeout->push_back(pipes_nlp_ns[HOST_POD_SWITCH(dest)][dest]);

                // reverse path for RTS packets
                routeback = new Route();
                
                routeback->push_back(queues_ns_nlp[dest][HOST_POD_SWITCH(dest)]);
                routeback->push_back(pipes_ns_nlp[dest][HOST_POD_SWITCH(dest)]);

                routeback->push_back(queues_nlp_nup[HOST_POD_SWITCH(dest)][upper2]);
                routeback->push_back(pipes_nlp_nup[HOST_POD_SWITCH(dest)][upper2]);

                routeback->push_back(queues_nup_nc[upper2][core]);
                routeback->push_back(pipes_nup_nc[upper2][core]);

                //now take the only link back down to the src server!
                
                routeback->push_back(queues_nc_nup[core][upper]);
                routeback->push_back(pipes_nc_nup[core][upper]);

                routeback->push_back(queues_nup_nlp[upper][HOST_POD_SWITCH(src)]);
                routeback->push_back(pipes_nup_nlp[upper][HOST_POD_SWITCH(src)]);

                routeback->push_back(queues_nlp_ns[HOST_POD_SWITCH(src)][src]);
                routeback->push_back(pipes_nlp_ns[HOST_POD_SWITCH(src)][src]);

                routeout->set_reverse(routeback);
                routeback->set_reverse(routeout);
            
                paths->push_back(routeout);
            }
            return paths;
        }
    }
}


int IncFatTreeTopology::find_lp_switch(AbstractQueue* queue){
    //first check ns_nlp
    for (int i=0;i<NSRV;i++)
        for (int j = 0;j<NK;j++)
        if (queues_ns_nlp[i][j]==queue)
        return j;

    for (int i=0;i<NK;i++)
        for (int j = 0;j<NK;j++)
        if (queues_nup_nlp[i][j]==queue)
        return j;

    return -1;
}

int IncFatTreeTopology::find_up_switch(AbstractQueue* queue){

  //first check nc_nup
  for (int i=0;i<NC;i++)
    for (int j = 0;j<NK;j++)
      if (queues_nc_nup[i][j]==queue)
	return j;

  //check nlp_nup
  for (int i=0;i<NK;i++)
    for (int j = 0;j<NK;j++)
      if (queues_nlp_nup[i][j]==queue)
	return j;

  return -1;
}

int IncFatTreeTopology::find_core_switch(AbstractQueue* queue){
    //first check nup_nc
    for (int i=0;i<NK;i++)
        for (int j = 0;j<NC;j++)
        if (queues_nup_nc[i][j]==queue)
        return j;

    return -1;
}

int IncFatTreeTopology::find_destination(AbstractQueue* queue){
    //first check nlp_ns
    for (int i=0;i<NK;i++)
        for (int j = 0;j<NSRV;j++)
        if (queues_nlp_ns[i][j]==queue)
        return j;
    return -1;
}

void IncFatTreeTopology::print_path(std::ofstream &paths,int src,const Route* route){
    paths << "SRC_" << src << " ";
    
    if (route->size()/2==2){
        paths << "LS_" << find_lp_switch((Queue*)route->at(1)) << " ";
        paths << "DST_" << find_destination((Queue*)route->at(3)) << " ";
    } else if (route->size()/2==4){
        paths << "LS_" << find_lp_switch((Queue*)route->at(1)) << " ";
        paths << "US_" << find_up_switch((Queue*)route->at(3)) << " ";
        paths << "LS_" << find_lp_switch((Queue*)route->at(5)) << " ";
        paths << "DST_" << find_destination((Queue*)route->at(7)) << " ";
    } else if (route->size()/2==6){
        paths << "LS_" << find_lp_switch((Queue*)route->at(1)) << " ";
        paths << "US_" << find_up_switch((Queue*)route->at(3)) << " ";
        paths << "CS_" << find_core_switch((Queue*)route->at(5)) << " ";
        paths << "US_" << find_up_switch((Queue*)route->at(7)) << " ";
        paths << "LS_" << find_lp_switch((Queue*)route->at(9)) << " ";
        paths << "DST_" << find_destination((Queue*)route->at(11)) << " ";
    } else {
        paths << "Wrong hop count " << route->size()/2;
    }
    
    paths << endl;
}

/**
 * Changes queue type (mainly for MTP purposes)
*/
AbstractQueue* IncFatTreeTopology::change_queue_s_uplink( int s, AbstractQueueLogger* queueLogger, bool og, double thresholdp, string fp){
    int k = s / (K / 2); // setup calculation in init_network (k = s * k/2 + l)
    // only uplink
    if(!og){
        queues_ns_nlp[s][k] = new MsgQueue(speedFromMbps((double)HOST_NIC), _queuesize, *eventlist, queueLogger, thresholdp, fp);
    }else{
        queues_ns_nlp[s][k] = new MsgQueueOg(speedFromMbps((double)HOST_NIC), _queuesize, *eventlist, queueLogger, fp);
    }

    std::string name = "SRC" + ntoa(s) + "->LS" +ntoa(k);
    queues_ns_nlp[s][k]->setName(name);
    logfile->writeName(*(queues_ns_nlp[s][k]));
    return queues_ns_nlp[s][k];
}

AbstractQueue* IncFatTreeTopology::change_queue_s_downlink( int s, AbstractQueueLogger* queueLogger, bool og, double thresholdp, string fp){
    int k = s / (K / 2); // setup calculation in init_network (k = s * k/2 + l)
    // only downlink
    if(!og){
        queues_nlp_ns[k][s] = new MsgQueue(speedFromMbps((double)HOST_NIC), _queuesize, *eventlist, queueLogger, thresholdp, fp);
    }else{
        queues_nlp_ns[k][s] = new MsgQueueOg(speedFromMbps((double)HOST_NIC), _queuesize, *eventlist, queueLogger, fp);
    }
    std::string name = "LS" + ntoa(k) + "->DST" +ntoa(s);
    queues_nlp_ns[k][s]->setName(name);
    logfile->writeName(*(queues_nlp_ns[k][s]));
    return queues_nlp_ns[k][s];
}

void IncFatTreeTopology::change_queue_lp_uplink( int j, AbstractQueueLogger* queueLogger, bool og, double thresholdp, string fp){
    int podid = 2 * j / K;

    // accessibility layer can send to either switch on aggregation layer
    for(int k = MIN_POD_ID(podid); k <= MAX_POD_ID(podid); k++){ 
        // only uplink
        if(!og){
            queues_nlp_nup[j][k] = new MsgQueue(speedFromMbps((double)HOST_NIC), _queuesize, *eventlist, queueLogger, thresholdp, fp);
        }else{
            queues_nlp_nup[j][k] = new MsgQueueOg(speedFromMbps((double)HOST_NIC), _queuesize, *eventlist, queueLogger, fp);
        }
        std::string name = "LS" + ntoa(j) + "->US" +ntoa(k);
        queues_nlp_nup[j][k]->setName(name);
        logfile->writeName(*(queues_nlp_nup[j][k]));
    }
}

void IncFatTreeTopology::change_queue_lp_downlink( int j, AbstractQueueLogger* queueLogger, bool og, double thresholdp, string fp){
    int podid = 2 * j / K;

    for(int k = MIN_POD_ID(podid); k <= MAX_POD_ID(podid); k++){ 
        // only downlink 
        if(!og){
            queues_nup_nlp[k][j] = new MsgQueue(speedFromMbps((double)HOST_NIC), _queuesize, *eventlist, queueLogger, thresholdp, fp);
        }else{
            queues_nup_nlp[k][j] = new MsgQueueOg(speedFromMbps((double)HOST_NIC), _queuesize, *eventlist, queueLogger, fp);
        }
        std::string name = "UP" + ntoa(k) + "->LS" +ntoa(j);
        queues_nup_nlp[k][j]->setName(name);
        logfile->writeName(*(queues_nup_nlp[k][j]));
    }
}