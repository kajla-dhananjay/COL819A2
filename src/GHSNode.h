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

struct Message
{
  //msg[0] = sender_id;
  //msg...... = actual message
  std::vector<std::string> msg;
  Message(std::vector<std::string> m)
  {
    msg = m;
  }
};

/** @brief Delivers a thread-safe queue
 */

struct Queue
{
  std::mutex mut;
  std::queue<Message *> q;
  void push(Message *m)
  {
    mut.lock();
    q.push(m);
    mut.unlock();
  }
  Message *top()
  {
    mut.lock();
    Message *temp = q.front();
    mut.unlock();
    return temp;
  }
  Message *pop()
  {
    mut.lock();
    Message *temp = q.front();
    q.pop();
    mut.unlock();
    return temp;
  }
  bool empty()
  {
    mut.lock();
    bool z = q.empty(); //This should be atomic
    mut.unlock();
    return z;
  }
};

/** @brief Provides Networking Functionality between nodes
 */

struct Network
{
  std::unordered_map<int, Queue > msg_queues;
};

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
    std::ofstream ofs; //!< File Stream to write errors and debug info into

    std::set<std::pair<int, int> > basic, branch, reject; //!< Set containing (edge_weight, id) of edges in basic, branch and reject state respectively
    
    std::string state; //!< Represents state of the node. Possible states : [ "sleep", "find", "found"]
    std::string name; //!< Represents name of the fragment of which current node is a part of
    std::unordered_map<int, std::string> stat; //!< Represents state of the edge with given neighbor. Possible states : ["basic", "branch", "reject"]
    std::unordered_map<int, int> nbd_list; //!< Adjacency list rep. of neighbors of current node.
    
    int nodeid; //!< Nodeid of the current node
    int level; //!< Level of the fragment of which current node is a part of
    int best_edge; //!< Temp Variable
    int best_weight; //!< Temp Variable
    int test_edge; //!< Temp Variable
    int find_count; //!< Temp Variable 
    int in_branch; //!< Temp Variable 



    /************Message Handlers*****************/

    void handleConnect();
    void handleInitiate();
    void handleTest();
    void handleAccept();
    void handleReject();
    void handleReport();
    void handleChangeroot();

    /*********************************************/

    void initialize(); //!< Runs Algorithm 1
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
    Message *msgCreater(std::vector<std::string> msg); //!< Creates a message for given string vector by adding header info (nodeid)
    void messagePrinter(); //!< Prints messages into ofs in a human-friendly way
  
  public:
    
    GHSNode(int nid, std::unordered_map<int, int> neighbors, Network *net, IsComplete *iscom); //!< Constructor to initialize the node
    void run(); //!< Public Function to let the thread_runner run the GHS node
    std::vector<int> getMSTEdges();
    void printNode(std::string id); //!< Prints the node into ofs
    
};
#endif
