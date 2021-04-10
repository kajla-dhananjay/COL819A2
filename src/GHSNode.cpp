#include <bits/stdc++.h>
#include "Graph.h"
#include "GHSNode.h"
using namespace std::chrono;

GHSNode::GHSNode(int nid, std::unordered_map<int, int> neighbors, Network *net)
{
  this->nodeid = nid;
  this->nbd_list = neighbors;
  this->network = net;
  this->ofs.open((std::to_string(nid) + ".txt").c_str());
}

void GHSNode::run()
{
#ifdef Debug
  //std::cerr << "Node id = " << nodeid << " has Thread id = " << pthread_self() << std::endl;
#endif

  auto start = high_resolution_clock::now();
 
  //std::cerr << "A" << std::endl;


  Message *mes = msgCreater({"Hi I am your neighbor with nodeid = " + std::to_string(nodeid)});
  
  //std::cerr << "F" << std::endl;
  
  sendMessage(1 - nodeid, mes); 
  
  //std::cerr << "B" << std::endl;
  
  while((duration_cast<seconds>(high_resolution_clock::now() - start)).count() < nodeid + 2 )
  {
    continue;
  }
  
  bool msgRecieved = false;

  while((duration_cast<seconds>(high_resolution_clock::now() - start)).count() < 10 && !msgRecieved)
  {
    if(recieveMessage())
    {
      messagePrinter();
      break;
    }
    continue;
  }
  //std::cerr << "Ended the wait after time : " << (duration_cast<seconds>(high_resolution_clock::now() - start)).count() << " for thread " << pthread_self() << std::endl;
  //initialize();
  //runner();
}

int GHSNode::findMinEdge()
{
  int min_wt = INF;
  int min_node_id = -1;

  for(auto it : nbd_list)
  {
    if(stat[it.first] != "basic")
    {
      continue;
    }

    if(it.second < min_wt)
    {
      min_wt = it.second;
      min_node_id = it.first;
    }
  }
  return min_node_id;
}

Message *GHSNode::msgCreater(std::vector<std::string> msg)
{
  //std::cerr << "C" << std::endl;
  std::vector<std::string> m;
  m.push_back(std::to_string(nodeid));
  for(auto it : msg)
  {
    m.push_back(it);
  }
  //std::cerr << "D" << std::endl;
  Message *mg = new Message(m);
  //std::cerr << "E" << std::endl;
  return mg;
}

void GHSNode::initialize()
{
  // Label all edges basic

  for(auto it : nbd_list)
  {
    stat[it.first] = "basic";
  }

  // Find pq as the least weight edge from p

  int q = findMinEdge();
  stat[q] = "branch";
  level = 0;
  state = "found";
  sendMessage(q, msgCreater({"connect",std::to_string(level)}));
}

void GHSNode::runner()
{
  //while(Queue is empty)
  //{
  //  wait
  //}
  //DoSomething();
}

void GHSNode::sendMessage(int dest, Message *m)
{
  network->msg_queues[dest].push(m);
}


void GHSNode::messagePrinter()
{
  if(msg == NULL)
  {
    ofs << "No Valid Message Found" << std::endl;
    return;
  }
  ofs << "MESSAGE from Node : " << msg->msg[0] << std::endl;
  for(int i = 1; i < (int)((msg->msg).size()); i++)
  {
    ofs << "Row " << i << " : " << (msg->msg)[i] << std::endl;
  }
}

bool GHSNode::recieveMessage()
{
  if((network->msg_queues)[nodeid].empty())
  {
    msg = NULL;
    return false;
  }
  else 
  {
    msg = (network->msg_queues)[nodeid].pop();
    return true;
  }
}
