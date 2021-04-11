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

/** @brief Defines the structure of a single node in GHS Algorithm 
 */ 

class GHSNode
{
  private:
    
    Network *network; //!< Access point to the global network

    std::string state; //!< Represents state of the node. Possible states : [ "sleep", "find", "found"]
    std::string name; //!< Represents name of the fragment of which current node is a part of
    std::unordered_map<int, std::string> stat; //!< Represents state of the edge with given neighbor. Possible states : ["basic", "branch", "reject"]
    std::set<std::pair<int, int> > basic, branch, reject; //!< Set containing (edge_weight, id) of edges in basic, branch and reject state respectively
    std::unordered_map<int, int> nbd_list; //!< Adjacency list rep. of neighbors of current node.
    int nodeid; //!< Nodeid of the current node
    int level; //!< Level of the fragment of which current node is a part of
    int best_edge; //!< Temp Variable
    int best_weight; //!< Temp Variable
    int test_edge;
    int find_count;
    int in_branch;

    bool hasmst; //!< Flag which rep whether the current node has all info to generate MST
    
    Message *msg; //!< Stores the pointer to the most recently recieved message

    Graph<int, int>* mst; //!< If hasmst is true, this will be the MST of the global graph, otherwise this is NULL
  
    std::ofstream ofs; //!< File Stream to write errors and debug info into

    /************Message Handlers*****************/

    void handleConnect();
    void handleInitiate();
    void handleTest();
    void handleAccept();
    void handleReject();
    void handleReport();
    void handleChangeroot();

    /*********************************************/

    void test();
    void report();
    void changeRoot();

    void messagePrinter(); //!< Prints messages into ofs in a human-friendly way
    void initialize(); //!< Runs Algorithm 1
    void runner(); //!< Internal function to keep GHS Node active till MST has been found
    void sendMessage(int dest, Message *m); //!< Sends the given message to the given neighbor via network
    
    int findMinEdge(); //!< Finds the neighbor which has the minimum edge weight with current node among all "basic" neighbors
    bool recieveMessage(); //!< Checks to see if there is a message, if there is none, returns false, if there is a message, stores the message into msg.
  
    Message *msgCreater(std::vector<std::string> msg); //!< Creates a message for given string vector by adding header info (nodeid)
  
  public:
    void printNode(std::string id); //!< Prints the node into ofs
    void run(); //!< Public Function to let the thread_runner run the GHS node
    GHSNode(int nid, std::unordered_map<int, int> neighbors, Network *net); //!< Constructor to initialize the node
    bool hasMst(); //!< Returns the boolean variable hasmst
    Graph<int, int>* getMst(); //!< Returns the pointer to mst if stored
};
#endif
