#include <bits/stdc++.h>
#include "Graph.h"
#include "GHSNode.h"
using namespace std::chrono;

void GHSNode::handleConnect()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]);
  int l = std::stoi(mg[2]);
  if(l < level)
  {
    stat[q] = "branch";
    std::vector<std::string> m;
    m.push_back("initiate");
    m.push_back(std::to_string(level));
    m.push_back(name);
    m.push_back(state);
    sendMessage(q, msgCreater(m));
  }
  else if(stat[q] == "basic")
  {

  }
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
  std::vector<std::string> m;
  m.push_back(std::to_string(nodeid));
  for(auto it : msg)
  {
    m.push_back(it);
  }
  
  Message *mg = new Message(m);
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
  rec = 0;
  std::vector<std::string> st;
  st.push_back("connect");
  st.push_back(std::to_string(level));
  sendMessage(q, msgCreater(st));
}

void GHSNode::runner()
{
  while(!recieveMessage())
  {
    continue;
  }
  if((msg->msg).size() < 2)
  {
    if((msg->msg).size() == 0)
    {
      std::cerr << "Completely Empty Message Sent" << std::endl;
    }
    else
    {
      std::cerr << "Empty Message Sent from node : " << (msg->msg)[0] << std::endl;
    }
    exit(46);
  }
  std::string mval = (msg->msg)[1];
  if(mval == "connect")
  {
    handleConnect();
  }
  else if(mval == "initiate")
  {
    handleInitiate();
  }
  else if(mval == "test")
  {
    handleTest();
  }
  else if(mval == "reject")
  {
    handleReject();
  }
  else if(mval == "accept")
  {
    handleAccept();
  }
  else if(mval == "report")
  {
    handleReport();
  }
  else if(mval == "changeroot")
  {
    handleChangeroot();
  }
}

void GHSNode::sendMessage(int dest, Message *m)
{
  network->msg_queues[dest].push(m);
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

void GHSNode::printNode(std::string id)
{
  ofs << "PRINTING requested by process : " << id << std::endl << std::endl;
  ofs << "Generic Information : " << std::endl << std::endl;
  ofs << "Nodeid : " << nodeid << std::endl;
  ofs << "State : " << state << std::endl;
  ofs << "Name : " << name << std::endl;
  ofs << "Level : " << level << std::endl;
  ofs << "hasmst : " << hasmst << std::endl;

  ofs << "\nEdge States : " << std::endl << std::endl;

  for(auto it : stat)
  {
    ofs << "Neighbor id : " << it.first << " | Edge State : " << it.second << std::endl;
  }

  ofs << "\nNode Printing ENDED" << std::endl << std::endl;
}


void GHSNode::run()
{
#ifdef Debug
  //std::cerr << "Node id = " << nodeid << " has Thread id = " << pthread_self() << std::endl;
#endif

  //auto start = std::chrono::high_resolution_clock::now();

  initialize();
  //while(!recieveMessage() && duration_cast<seconds>(std::chrono::high_resolution_clock::now() - start).count() < 10)
  //{
    //continue;
  //}
  //messagePrinter();
  runner();
}


GHSNode::GHSNode(int nid, std::unordered_map<int, int> neighbors, Network *net)
{
  this->nodeid = nid;
  this->nbd_list = neighbors;
  this->network = net;
  this->ofs.open((std::to_string(nid) + ".txt").c_str());
  this->mst = NULL;
  this->hasmst = false;
}

bool GHSNode::hasMst()
{
  return hasmst;
}
 
Graph<int, int> * GHSNode::getMst()
{
  return mst; 
}
