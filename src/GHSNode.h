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
    int nodeid;
    Network *network;
    std::string state;
    std::unordered_map<int, std::string> stat;
    std::unordered_map<int, int> nbd_list;
    std::string name;
    int level;
    GHSNode *parent;
    int bestWt;
    int rec;
    GHSNode *bestNode;
    GHSNode *testNode;

    int findMinEdge();
    Message msgCreater(std::vector<std::string> msg);
    void initialize();
    Graph<int, int>* runner();
    void sendMessage(int dest, Message m);
    Message* recieveMessage();
  public:
    Graph<int, int> *run();
    GHSNode(int nid, std::unordered_map<int, int> neighbors);
};
#endif
