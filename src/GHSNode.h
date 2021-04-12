#include<bits/stdc++.h>
#include "Graph.h"

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
  std::vector<std::string> msg;   
public:
  Message(std::vector<std::string> m)
  {
    msg = m;
  }
  std::vector<std::string> getMessage()
  {
    return msg;
  }
};

/** @brief Delivers a thread-safe queue
 */

class Queue
{
private:
  pthread_mutex_t mut;
  std::queue<Message *> q;
  int queue_size;
public:
  Queue()
  {
    queue_size = 0;
  }
  void push(Message *m)
  {
    pthread_mutex_lock(&mut);
    q.push(m); //This should be atomic
    pthread_mutex_unlock(&mut);
  }
  Message *front()
  {
    pthread_mutex_lock(&mut);
    if(queue_size == 0)
    {
      pthread_mutex_unlock(&mut);
      return NULL;
    }
    Message *temp = q.front(); //This should be atomic
    pthread_mutex_unlock(&mut);
    return temp;
  }
  Message *pop()
  {
    pthread_mutex_lock(&mut);
    if(queue_size == 0)
    {
      pthread_mutex_unlock(&mut);
      return NULL;
    }
    Message *temp = q.front();
    q.pop();
    pthread_mutex_unlock(&mut);
    return temp;
  }
  bool empty()
  {
    pthread_mutex_lock(&mut);
    int temp = queue_size;
    pthread_mutex_unlock(&mut);
    return (temp == 0);
  }
  int getQueueSize()
  {
    pthread_mutex_lock(&mut);
    int temp = queue_size;
    pthread_mutex_unlock(&mut);
    return temp;
  }
};

/** @brief Provides Networking Functionality between nodes
 */

class Network
{
private:
  std::unordered_map<int, Queue *> msg_queues;
public:
  Network(std::vector<int> nodes)
  {
    for(auto it : nodes)
    {
      Queue *q = new Queue();
      msg_queues[it] = q;
    }
  }
  Queue *getQueue(int i)
  {
    if(msg_queues.find(i) == msg_queues.end())
    {
      std::cerr << "BAD network Request" << std::endl;
    }
    return msg_queues[i];
  }
}
;

struct IsComplete
{
  bool complete;
  IsComplete()
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

    Message *prev_msg; //!< Previous message
    Message *prev_msg1;
    
    std::ofstream ofs; //!< File Stream to write errors and debug info into

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
    
    int findMinEdge(); //!< Finds the neighbor which has the minimum edge weight with current node among all "basic" neighbors
    void changestat(int node, std::string stateval);
    Message *msgCreater(std::vector<std::string> msg); //!< Creates a message for given string vector by adding header info (nodeid)
    void sentmessagePrinter(int dest, Message *msg); //!< Prints messages into ofs in a human-friendly way
    void recievedmessagePrinter(Message *msg); //!< Prints messages into ofs in a human-friendly way
  
  public:
    
    GHSNode(int nid, std::unordered_map<int, int> neighbors, Network *net, IsComplete *iscom); //!< Constructor to wakeup the node
    void run(); //!< Public Function to let the thread_runner run the GHS node
    std::vector<int> getMSTEdges();
    void printNode(std::string id); //!< Prints the node into ofs
    
};
#endif
