#include <bits/stdc++.h>
#include "Graph.h"
#include "GHSNode.h"
using namespace std::chrono;

void GHSNode::handleConnect()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]);
  int l = std::stoi(mg[2]);
  if(state == "sleep")
  {
    initialize();
  }
  if(l < level)
  {
    if(stat[q] == "basic")
    {
      basic.erase(basic.find(std::make_pair(nbd_list[q],q)));
    }
    else if(stat[q] == "reject")
    {
      std::cerr << "Something Fishy here" << std::endl;
      reject.erase(reject.find(std::make_pair(nbd_list[q],q)));
    }
    
    stat[q] = "branch";
    branch.insert(std::make_pair(nbd_list[q],q));

    std::vector<std::string> m;
    m.push_back("initiate");
    m.push_back(std::to_string(level));
    m.push_back(name);
    m.push_back(state);

    sendMessage(q, msgCreater(m));
    
    if(state == "find")
    {
      find_count++;
    }
    
    free(msg);
    msg = NULL;
  }
  else if(stat[q] == "basic")
  {
    sendMessage(nodeid, msg);
  }
  else
  {
    std::vector<std::string> st;
    st.push_back("initiate");
    st.push_back(std::to_string(level+1));
    st.push_back(std::to_string(nbd_list[q]));
    st.push_back("find");
    sendMessage(q, msgCreater(st));
    free(msg);
    msg = NULL;
  }
}

void GHSNode::handleInitiate()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]);
  int l = std::stoi(mg[2]);
  std::string nm = mg[3];
  std::string st = mg[4];
  level = l;
  name = nm;
  state = st;
  in_branch = q;
  best_edge = -1;
  best_weight = INF;
  for(auto it : stat)
  {
    if(it.first == q || it.second != "branch")
    {
      continue;
    }
    std::vector<std::string> st;
    st.push_back("initiate");
    st.push_back(std::to_string(level));
    st.push_back(name);
    st.push_back(state);
    sendMessage(it.first, msgCreater(st));
    if(state == "find")
    {
      find_count++;
    }
  }
  if(state == "find")
  {
    test();
  }
  free(msg);
  msg = NULL;
}

void GHSNode::handleTest()
{
  std::vector<std::string> mg = (msg->msg);
  if(state == "sleep")
  {
    initialize();
  }
  int q = std::stoi(mg[0]);
  int l = std::stoi(mg[2]);
  std::string nm = mg[3];
  if(l > level)
  {
    sendMessage(nodeid, msg);
  }
  else if(nm != name)
  {
    std::vector<std::string> st;
    st.push_back("accept");
    sendMessage(q,msgCreater(st));
    free(msg);
    msg = NULL;
  }
  else
  {
    if(stat[q] == "basic")
    {
      basic.erase(basic.find(std::make_pair(nbd_list[q],q)));
      stat[q] = "reject";
      reject.insert(std::make_pair(nbd_list[q],q));
    }
    if(test_edge != q)
    {
      std::vector<std::string> st;
      st.push_back("reject");
      sendMessage(q, msgCreater(st));
    }
    else
    {
      test();
    }
    free(msg);
    msg = NULL;
  }
}

void GHSNode::handleAccept()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]);
  test_edge = -1;
  if(best_weight > nbd_list[q])
  {
    best_edge = q;
    best_weight = nbd_list[q];
  }
  free(msg);
  msg = NULL;
  report();
}

void GHSNode::handleReject()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]);
  if(stat[q] == "basic")
  {
    basic.erase(basic.find(std::make_pair(nbd_list[q],q)));
    stat[q] = "reject";
    reject.insert(std::make_pair(nbd_list[q],q));
  }
  free(msg);
  msg = NULL;
  test();
}

void GHSNode::handleReport()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]);
  int w = std::stoi(mg[2]);
  if(q != in_branch)
  {

  }
  else if(state == "find")
  {
    sendMessage(nodeid, msg);
  }
  else if(w > best_weight)
  {
    changeRoot();
    free(msg);
    msg = NULL;
  }
  else if(w == best_weight && w == INF)
  {
    //HALT
    free(msg);
    msg = NULL;
  }
}

void GHSNode::handleChangeroot()
{
  free(msg);
  msg = NULL;
  changeRoot();
}

void GHSNode::changeRoot()
{
  if(stat[best_edge] == "branch")
  {
    std::vector<std::string> st;
    st.push_back("changeRoot");
    sendMessage(best_edge, msgCreater(st));
  }
  else
  {
    std::vector<std::string> st;
    st.push_back("connect");
    st.push_back(std::to_string(level));
    sendMessage(best_edge, msgCreater(st));
    if(stat[best_edge] == "basic")
    {
      basic.erase(basic.find(std::make_pair(nbd_list[best_edge], best_edge)));
    }
    if(stat[best_edge] == "reject")
    {
      reject.erase(reject.find(std::make_pair(nbd_list[best_edge], best_edge)));
    }
    stat[best_edge] = "branch";
    branch.insert(std::make_pair(nbd_list[best_edge], best_edge));
  }
}

void GHSNode::report()
{
  if(find_count == 0 && test_edge == -1)
  {
    state = "found";
    std::vector<std::string> st;
    st.push_back("report");
    st.push_back(std::to_string(best_weight));
    sendMessage(in_branch,msgCreater(st));
  }
}

void GHSNode::test()
{
  test_edge = findMinEdge();
  if(test_edge != -1)
  {
    std::vector<std::string> st;
    st.push_back("test");
    st.push_back(std::to_string(level));
    st.push_back(name);
    sendMessage(test_edge, msgCreater(st));
  }
  else
  {
    report();
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
  if(basic.empty())
  {
    return -1;
  }
  return (*basic.begin()).second;
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
    basic.insert(std::make_pair(nbd_list[it.first],it.first));
  }

  // Find pq as the least weight edge from p

  int q = findMinEdge();
  basic.erase(basic.find(std::make_pair(nbd_list[q],q)));
  branch.insert(std::make_pair(nbd_list[q],q));
  stat[q] = "branch";
  level = 0;
  state = "found";
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
  this->state = "sleep";
}

bool GHSNode::hasMst()
{
  return hasmst;
}
 
Graph<int, int> * GHSNode::getMst()
{
  return mst; 
}
