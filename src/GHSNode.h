#include<bits/stdc++.h>

/** @file GHSNode.h
 * Header File for GHSNodes
 */

#ifndef _GHSNODE_H_
#define _GHSNODE_H_

#define INF std::numeric_limits<int>::max()

/** @brief Provides a message interface
 */

class Message
{
private:
  std::vector<std::string> msg; //!< message content 
public:
  Message(std::vector<std::string> m) //!< Initiates a message
  {
    msg = m;
  }
  std::vector<std::string> getMessage() //!< returns the message
  {
    return msg;
  }
};

/** @brief Delivers a thread-safe queue
 */

class Queue
{
private:
  std::mutex mut; //!< mutex lock for thread safety
  std::queue<Message *> q; //!< Message queue
  int queueid; //!< Id of the node to which this queue belongs
public:
  Queue()
  {
    queueid = -1; //!< Initialized as bad node
  }
  Queue(int qid)
  {
    queueid = qid; //!< ID of the queue
  }
  int getqueueid()
  {
    return queueid; //!< returns id for the node which owns this queue
  }
  void push(Message *m)
  {
    mut.lock(); //!< Puts the lock in place
    q.push(m); //!< This push operation is atomic
    mut.unlock(); //!< Unlocks the queue
  }
  Message *front()
  {
    mut.lock(); //!< Puts the lock in place
    if(q.size() == 0)
    {
      mut.unlock(); //!< Unlocks the queue
      return NULL; //!< Returns NULL if queue is empty
    }
    Message *temp = q.front(); //!< This front operation is atomic
    mut.unlock(); //!< Unlocks the queue
    return temp;
  }
  Message *pop()
  {
    mut.lock(); //!< Puts the lock in place
    int temp = q.size(); //!< Temporary variable to store queue size
    if(temp == 0)
    {
      mut.unlock(); //!< Unlocks the queue
      return NULL; //!< Returns NULL if queue is empty
    }
    Message *tmp = q.front(); //!< Temporary variable to store the front of queue
    q.pop(); //!< Pops the queue
    if((int)q.size() != temp - 1) //!< Pop should change the size of queue by 1
    {
      std::cerr << "QUEUE Error" << std::endl; 
      exit(1);
    }
    mut.unlock(); //!< Unlocks the queue
    return tmp;
  }
  bool empty()
  {
    mut.lock(); //!< Puts the lock in place
    int temp = q.size(); //!< Temporary variable for queue size
    mut.unlock(); //!< Unlocks the queue
    return (temp == 0); //!< Returns if queue is empty
  }
  int getQueueSize()
  {
    mut.lock(); //!< Puts the lock in place
    int temp = q.size(); //!< Size of queue
    mut.unlock(); //!< Unlocks the queue 
    return temp; //!< returns size of queue
  }
};

/** @brief Provides Networking Functionality between nodes
 */

class Network
{
private:
  std::unordered_map<int, Queue *> msg_queues; //!< Network queues
public:
  Network(std::vector<int> nodes) //!< Network Constructor 
  {
    for(auto it : nodes)
    {
      Queue *q = new Queue(it); //!< Initiates a new queue
      msg_queues[it] = q; //!< Initializes the hashmap
    }
  }
  Queue *getQueue(int i) //!< Get's the ith queue
  {
    if(msg_queues.find(i) == msg_queues.end())
    {
      std::cerr << "BAD network Request" << std::endl;
    }
    return msg_queues[i]; //!< Returns the pointer to ith queue
  }
}
;

/** @brief Flags if the algo is complete. 
 * In a real distributed system one can spread this message via 
 * rumor mongering, etc.
 */
struct IsComplete
{
  bool complete; //!< If set true, the threads exit
  IsComplete() //!< Constructor that sets the default value false
  {
    complete = false; 
  }
};

/** @brief Defines the structure of a single node in GHS Algorithm 
 */ 

class GHSNode
{
  private:
    
    Network *network; //!< Access point to the global network
    IsComplete *isc; //!< Access point to give halt signal to main thread
    Message *msg; //!< Stores the pointer to the most recently recieved message
    
    Queue *nodequeue;
    
    std::set<std::pair<int, int> > basic, branch, reject; //!< Set containing (edge_weight, id) of edges in basic, branch and reject state respectively
    
    int nodeid; //!< Nodeid of the current node
    std::string SN; //!< Represents state of the node. Possible states : [ "sleep", "find", "found"]
    int LN; //!< Level of the fragment of which current node is a part of
    std::string FN; //!< Represents name of the fragment of which current node is a part of
    
    std::unordered_map<int, std::string> SE; //!< Represents state of the edge with given neighbor. Possible states : ["basic", "branch", "reject"]
    std::unordered_map<int, int> nbd; //!< Adjacency list rep. of neighbors of current node.
    
    int best_edge; //!< Temp Variable
    int best_weight; //!< Temp Variable
    int test_edge; //!< Temp Variable
    int in_branch; //!< Temp Variable 
    int find_count; //!< Temp Variable 



    /************Message Handlers*****************/

    void handleConnect();
    void handleInitiate();
    void handleTest();
    void handleAccept();
    void handleReject();
    void handleReport();
    void handleChangeroot();

    /*********************************************/

    void wakeup();
    void test();       
    void report();
    void changeRoot();

    /********************************************/

    void runner(); //!< REPL type function 
    
    /********************************************/

    void sendMessage(int dest, Message *m); //!< Sends the given message to the given neighbor via network
    bool recieveMessage(); //!< Checks to see if there is a message, if there is none, returns false, if there is a message, stores the message into msg.
  
    /********************************************/
   
    void tester();

    int findMinEdge(); //!< Finds the neighbor which has the minimum edge weight with current node among all "basic" neighbors
    void changestat(int node, std::string stateval);
    Message *msgCreater(std::vector<std::string> msg); //!< Creates a message for given string vector by adding header info (nodeid)
  
  public:
    
    GHSNode(int nid, std::unordered_map<int, int> &neighbors, Network *net, IsComplete *iscom); //!< Constructor to wakeup the node
    void run(); //!< Public Function to let the thread_runner run the GHS node
    std::vector<int> getMSTEdges(); //!< Returns all the branch edges
    
};
#endif
