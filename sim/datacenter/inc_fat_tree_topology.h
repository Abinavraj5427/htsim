#ifndef INC_FAT_TREE
#define INC_FAT_TREE
#include "topology.h"
#include "main.h"
#include "pipe.h"
#include "switch.h"
#include "msgqueue.h"
#include "msgqueueog.h"
#include "logfile.h"
#include <ostream>


#ifndef QT
#define QT
typedef enum {RANDOM, ECN, COMPOSITE, CTRL_PRIO, LOSSLESS, LOSSLESS_INPUT, LOSSLESS_INPUT_ECN, NORMAL} queue_type;
#endif

/**
 *      FAT TREE TOPOLOGY
 * 
 *        C-----------C            CORE LAYER (K*K/4)
 *       / \         / \
 *      /   \       /   \
 *     UP---UP     UP---UP         AGGREGATION LAYER (K*K/2)
 *     |  X  |     |  X  |
 *     LP---LP     LP---LP         ACCESSIBILITY LAYER (K*K/2)
 *    / \   / \   / \   / 
 *   1   2 3   4 5   6 7           :wSERVERS (K*K*K/4)
 * 
*/

#define MIN_POD_ID(pod_id) (pod_id*K/2)
#define MAX_POD_ID(pod_id) ((pod_id+1)*K/2-1)

#define HOST_POD_SWITCH(src) (2*src/K)
#define HOST_POD(src) (src/NC)

class IncFatTreeTopology: public Topology {
    public: 
        vector <Switch*> switches_lp; // lower pod switches
        vector <Switch*> switches_up; // upper pod switches
        vector <Switch*> switches_c;  // core switches

        vector< vector<Pipe*> > pipes_nc_nup;
        vector< vector<Pipe*> > pipes_nup_nlp;
        vector< vector<Pipe*> > pipes_nlp_ns;
        vector< vector<AbstractQueue*> > queues_nc_nup;
        vector< vector<AbstractQueue*> > queues_nup_nlp;
        vector< vector<AbstractQueue*> > queues_nlp_ns;

        vector< vector<Pipe*> > pipes_nup_nc;
        vector< vector<Pipe*> > pipes_nlp_nup;
        vector< vector<Pipe*> > pipes_ns_nlp;
        vector< vector<AbstractQueue*> > queues_nup_nc;
        vector< vector<AbstractQueue*> > queues_nlp_nup;
        vector< vector<AbstractQueue*> > queues_ns_nlp;

        IncFatTreeTopology(int no_of_nodes, mem_b queuesize, Logfile* log,EventList* ev, queue_type q);
        // inits queues/pipes
        void init_network();

        // get paths from src to dest
        virtual vector<const Route*>* get_paths(int src, int dest);

        AbstractQueue* alloc_src_queue(QueueLogger* q);
        AbstractQueue* alloc_queue(QueueLogger* q, mem_b queuesize);
        AbstractQueue* alloc_queue(QueueLogger* q, uint64_t speed, mem_b queuesize);

        vector<int>* get_neighbours(int src) { return NULL;};
        void print_path(std::ofstream& paths,int src,const Route* route);
        void change_queue_lp_downlink(int j, AbstractQueueLogger* queueLogger, bool og, double thresholdp, std::string fp);
        void change_queue_lp_uplink(int j, AbstractQueueLogger* queueLogger, bool og, double thresholdp, std::string fp);
        AbstractQueue* change_queue_s_downlink(int j, AbstractQueueLogger* queueLogger, bool og, double thresholdp, std::string fp);
        AbstractQueue* change_queue_s_uplink(int j, AbstractQueueLogger* queueLogger, bool og, double thresholdp, std::string fp);
        
    private:
        int find_lp_switch(AbstractQueue* queue);
        int find_up_switch(AbstractQueue* queue);
        int find_core_switch(AbstractQueue* queue);
        int find_destination(AbstractQueue* queue);
        void set_params(int no_of_nodes);
        int K, NK, NC, NSRV;
        int _no_of_nodes;
        mem_b _queuesize;
        queue_type qt;
        EventList* eventlist;
        Logfile* logfile;
};

#endif