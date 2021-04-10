#include<bits/stdc++.h>
#include "Graph.h"

#ifndef _GHSNODE_H_
#define _GHSNODE_H_

#define INF std::numeric_limits<int>::max()

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

struct Queue
{
  std::mutex mut;
  std::queue<Message> q;
  void push(Message m)
  {
    mut.lock();
    q.push(m);
    mut.unlock();
  }
  Message top()
  {
    mut.lock();
    Message temp = q.front();
    mut.unlock();
    return temp;
  }
  Message pop()
  {
    mut.lock();
    Message temp = q.front();
    q.pop();
    mut.unlock();
    return temp;
  }
};

struct Network
{
  std::unordered_map<int, Queue > msg_queues;
};

class GHSNode
{
  private:
    Network *network;
    std::string state;
    std::string name;
    std::unordered_map<int, std::string> stat;
    std::unordered_map<int, int> nbd_list;
    int nodeid;
    int level;
    int bestWt;
    int rec;
    
    bool hasMst;
    
    GHSNode *parent;
    GHSNode *bestNode;
    GHSNode *testNode;
    
    Graph<int, int>* mst;
    
    int findMinEdge();
    Message msgCreater(std::vector<std::string> msg);
    void initialize();
    void runner();
    void sendMessage(int dest, Message m);
    Message* recieveMessage();
  public:
    void run();
    Graph<int, int>* getMst();
    GHSNode(int nid, std::unordered_map<int, int> neighbors);
};
#endif
