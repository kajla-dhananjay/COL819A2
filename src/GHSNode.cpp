#include <bits/stdc++.h>
#include "Graph.h"
#include "GHSNode.h"
using namespace std::chrono;

void GHSNode::handleConnect()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]); //!< edge j
  int l = std::stoi(mg[2]); //!< L
  if(state == "sleep")      //!< if SN = Sleeping 
  {
    initialize();
  }
  if(l < level) //!< If L < LN
  {
    if(stat[q] == "basic") 
    {
      basic.erase(basic.find(std::make_pair(nbd_list[q],q)));
    }
    else if(stat[q] == "reject")
    {
      reject.erase(reject.find(std::make_pair(nbd_list[q],q)));
    }
    
    stat[q] = "branch"; //!< SE(j) <- Branch
    branch.insert(std::make_pair(nbd_list[q],q));

    std::vector<std::string> m; 
    m.push_back("initiate"); //!< Initiate()
    m.push_back(std::to_string(level)); //!< LN
    m.push_back(name); //!< FN
    m.push_back(state); //!< SN

    sendMessage(q, msgCreater(m)); //!< send Initiate(LN, FN, SN) on edge j 
    
    if(state == "find") //!< if SN = Find then
    {
      find_count++; //!< find-count <- find-count +1
    }
    
    free(msg);
    msg = NULL;
  }
  else if(stat[q] == "basic") //!< else if SE(j) = Basic
  {
    sendMessage(nodeid, msg); //!< place receieved message on end of queue
  }
  else
  {
    std::vector<std::string> st; 
    st.push_back("initiate"); //!< Initiate()
    st.push_back(std::to_string(level+1)); //!< LN +1
    st.push_back(std::to_string(nbd_list[q]));//!< w(j)
    st.push_back("find");//!< Find
    sendMessage(q, msgCreater(st)); //! send Initiate(LN+1, w(j), Find) on edge j
    free(msg);
    msg = NULL;
  }
}

void GHSNode::handleInitiate()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]); //!< edge j
  int l = std::stoi(mg[2]); //!< L
  std::string nm = mg[3]; //!< F
  std::string sta = mg[4]; //!< S
  level = l; //!< LN <- L
  name = nm; //!< FN <- F
  state = sta; //!< SN <- S
  in_branch = q; //!< in-branch <- j
  best_edge = -1; //!< best-edge <- nil
  best_weight = INF; //!< best-wt <- infinity
  for(auto it : stat) //!< forall
  {
    if(it.first == q || it.second != "branch") 
    {
      continue;
    }//!< i != j and SE(i) = branch
    std::vector<std::string> st; 
    st.push_back("initiate"); //!< Initiate()
    st.push_back(std::to_string(level)); //!< L
    st.push_back(name); //!< F
    st.push_back(state); //!< S
    sendMessage(it.first, msgCreater(st)); //!< send Initiate(L, F, S) on edge i
    if(state == "find") //!< if S = Find 
    {
      find_count++; //!< find-count <- find-count + 1
    }
  }
  if(state == "find") //!< if S = Find
  {
    test(); //!< execute procedure test
  }
  free(msg);
  msg = NULL;
}

void GHSNode::handleTest()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]); //!< edge j 
  if(state == "sleep") //!< If SN = Sleeping 
  {
    initialize(); //!< execute procedure wakeup
  }
  int l = std::stoi(mg[2]); //!< L
  std::string nm = mg[3]; //!< F
  if(l > level) //!< L > LN
  {
    sendMessage(nodeid, msg); //!< place recieved message on end of queue
  }
  else if(nm != name) //!< F != FN
  {
    std::vector<std::string> st;
    st.push_back("accept"); //!< Accept()
    sendMessage(q,msgCreater(st)); //!< Send Accept on edge j
    free(msg); 
    msg = NULL;
  }
  else
  {
    if(stat[q] == "basic") //!< if SE(j) = Basic
    {
      basic.erase(basic.find(std::make_pair(nbd_list[q],q)));
      stat[q] = "reject"; //!< SE(j) <- Rejected
      reject.insert(std::make_pair(nbd_list[q],q));
    }
    if(test_edge != q) //!< test-edge != j
    {
      std::vector<std::string> st;
      st.push_back("reject"); //!< Reject()
      sendMessage(q, msgCreater(st)); //!< send Reject on edge j
    }
    else
    {
      test(); //!< execute procedure test
    }
    free(msg);
    msg = NULL;
  }
}

void GHSNode::handleAccept()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]); //!< edge j
  test_edge = -1; //!< test-edge <- nil
  if(nbd_list[q] < best_weight) //!< w(j) < best-wt 
  {
    best_edge = q; //!< best-edge <- j
    best_weight = nbd_list[q]; //!< best-wt <- w(j)
  }
  free(msg);
  msg = NULL;
  report(); //!< execute procedure report()
}

void GHSNode::handleReject()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]); //!< edge j
  if(stat[q] == "basic") //!< if SE(j) = Basic 
  {
    basic.erase(basic.find(std::make_pair(nbd_list[q],q)));
    stat[q] = "reject"; //!< SE(j) = Reject
    reject.insert(std::make_pair(nbd_list[q],q));
  }
  free(msg);
  msg = NULL;
  test(); //!< execute procedure test()
}

void GHSNode::handleReport()
{
  std::vector<std::string> mg = (msg->msg);
  int q = std::stoi(mg[0]); //!< edge j
  int w = std::stoi(mg[2]); //!< w
  if(q != in_branch) //!< if j != in-branch
  {
    find_count--; //!< find-count <- find-count - 1
    if(w < best_weight) //!< w < best-wt
    {
      best_weight = w; //!< best-wt <- w
      best_edge = q; //!< best-edge <- j
    }
    free(msg);
    msg = NULL;
    report(); //!< execute procedure report()
  }
  else if(state == "find") //!< SN = Find
  {
    sendMessage(nodeid, msg); //!< place recieved message on end of queue
  }
  else if(w > best_weight) //!< w > best-wt
  {
    free(msg);
    msg = NULL;
    changeRoot(); //!< execute procedure change-root()
  }
  else if(w == best_weight && w == INF) //!< w = best-wt = infinity 
  {
    isc->complete = true; //!< halt
    free(msg);
    msg = NULL;
  }
}

void GHSNode::handleChangeroot()
{
  free(msg);
  msg = NULL;
  changeRoot(); //execute procedure change-root()
}

void GHSNode::changeRoot()
{
  if(stat[best_edge] == "branch") //!< SE(best-edge) = Branch
  {
    std::vector<std::string> st;
    st.push_back("changeRoot"); //!< Change-root()
    sendMessage(best_edge, msgCreater(st)); //!< send Change-root() on best-edge
  }
  else
  {
    std::vector<std::string> st;
    st.push_back("connect"); //!< Connect()
    st.push_back(std::to_string(level)); //!< LN
    sendMessage(best_edge, msgCreater(st)); //!< send Connect(LN) on best-edge
    if(stat[best_edge] == "basic")
    {
      basic.erase(basic.find(std::make_pair(nbd_list[best_edge], best_edge)));
    }
    if(stat[best_edge] == "reject")
    {
      reject.erase(reject.find(std::make_pair(nbd_list[best_edge], best_edge)));
    }
    stat[best_edge] = "branch"; //!< SE(best-edge) <- Branch
    branch.insert(std::make_pair(nbd_list[best_edge], best_edge));
  }
}

void GHSNode::report()
{
  if(find_count == 0 && test_edge == -1) //!< if find-count = 0 and test-edge = nil
  {
    state = "found"; //!< SN <- Found
    std::vector<std::string> st; 
    st.push_back("report"); //!< Report()
    st.push_back(std::to_string(best_weight)); //!< best-wt
    sendMessage(in_branch,msgCreater(st)); //!< send Report(best-wt) on in-branch
  }
}

void GHSNode::test()
{
  test_edge = findMinEdge(); //!< test-edge <- nil if no basic edges 
  //!< test-edge <- the minimum-weight adjacent edge in state Basic
  if(test_edge != -1) //!< If there are adjacent edges in the state Basic
  {
    std::vector<std::string> st;
    st.push_back("test"); //!< Test()
    st.push_back(std::to_string(level)); //!< LN
    st.push_back(name); //!< FN
    sendMessage(test_edge, msgCreater(st)); //!< send Test(LN, FN) on test-edge
  }
  else //!< Else test-edge <- nil (by default)
  {
    report(); //!< execute procedure report()
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
  for(auto it : nbd_list)
  {
    stat[it.first] = "basic";
    basic.insert(std::make_pair(it.second, it.first));
  }

  int q = findMinEdge(); 
  basic.erase(basic.find(std::make_pair(nbd_list[q],q)));
  stat[q] = "branch"; //!< SE(m) <- Branch;
  branch.insert(std::make_pair(nbd_list[q],q));
  level = 0; //!< LN <- 0
  state = "found"; //!< SN <- Found
  find_count = 0; //!< Find-count <- 0
  std::vector<std::string> st;
  st.push_back("connect");
  st.push_back(std::to_string(level));
  sendMessage(q, msgCreater(st)); //!< send Connect(0) 
}

void GHSNode::runner()
{
  while(!(isc->complete))
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
    else if(mval == "accept")
    {
      handleAccept();
    }
    else if(mval == "reject")
    {
      handleReject();
    }
    else if(mval == "report")
    {
      handleReport();
    }
    else if(mval == "changeroot")
    {
      handleChangeroot();
    }
    else
    {
      std::cerr << "Message Of Invalid Type" << std::endl;
    }
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

  ofs << "\nEdge States : " << std::endl << std::endl;

  for(auto it : stat)
  {
    ofs << "Neighbor id : " << it.first << " | Edge State : " << it.second << std::endl;
  }

  ofs << "\nNode Printing ENDED" << std::endl << std::endl;
}


void GHSNode::run()
{
  initialize();
  runner();
}
    
std::vector<int> GHSNode::getMSTEdges()
{
  std::vector<int> v;
  for(auto it : branch)
  {
    v.push_back(it.first);
  }
  return v;
}

GHSNode::GHSNode(int nid, std::unordered_map<int, int> neighbors, Network *net, IsComplete *iscom)
{
  this->nodeid = nid;
  this->nbd_list = neighbors;
  this->network = net;
  this->ofs.open((std::to_string(nid) + ".txt").c_str());
  this->state = "sleep";
  this->isc = iscom;
}

