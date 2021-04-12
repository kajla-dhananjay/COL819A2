#include <bits/stdc++.h>
#include "Graph.h"
#include "GHSNode.h"
using namespace std::chrono;

int GHSNode::findMinEdge()
{
  if(basic.empty())
  {
    return -1;
  }
  return (*basic.begin()).second;
}

void GHSNode::changestat(int node, std::string stateval)
{
  if(SE[node] == "basic")
  {
    basic.erase(basic.find(std::make_pair(nbd[node], node)));
  }
  else if(SE[node] == "branch")
  {
    branch.erase(branch.find(std::make_pair(nbd[node], node)));
  }
  else if(SE[node] == "reject")
  {
    reject.erase(reject.find(std::make_pair(nbd[node], node)));
  }
  else
  {
    std::cerr << "BAD EDGE STATE" << std::endl;
  }

  SE[node] = stateval;
  
  if(stateval == "basic")
  {
    basic.insert(std::make_pair(nbd[node], node));
  }
  else if(stateval == "branch")
  {
    branch.insert(std::make_pair(nbd[node], node));
  }
  else if(stateval == "reject")
  {
    reject.insert(std::make_pair(nbd[node], node));
  }
  else
  {
    std::cerr << "BAD EDGE STATEVAL" << std::endl;
  }
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

void GHSNode::sendMessage(int dest, Message *m)
{
  sentmessagePrinter(dest, m);
  Queue *q = network->getQueue(dest);
  std::cerr << "Node id : " << nodeid << " sending message to node with id : " << dest << " with queue at address : " << m << std::endl;
  q->push(m);
}


bool GHSNode::recieveMessage()
{
  msg = nodequeue->pop();
  recievedmessagePrinter(msg);
  return (msg != NULL);
}



void GHSNode::wakeup()
{
  if(nbd.empty())
  {
    isc->complete = true;
  }
  
  for(auto it : nbd)
  {
    SE[it.first] = "basic";
    basic.insert(std::make_pair(it.second, it.first));
  }

  int m = findMinEdge(); // let m be adjacent edge of minimum weight;
  changestat(m, "branch"); //!< SE(m) <- Branch
  LN = 0; //!< LN <- 0
  SN = "found"; //!< SN <- Found
  find_count = 0; //!< Find-count <- 0
  std::vector<std::string> st;
  st.push_back("connect");
  st.push_back(std::to_string(LN));
  sendMessage(m, msgCreater(st)); //!< send Connect(0) on edge m 
  std::cerr << "Completed wakeup() on node with id : " << nodeid << std::endl;
}

void GHSNode::handleConnect()
{
  std::vector<std::string> mg = msg->getMessage();
  int j = std::stoi(mg[0]); //!< edge j
  int L = std::stoi(mg[2]); //!< L
  if(SN == "sleep")      //!< if SN = Sleeping 
  {
    wakeup(); //!< execute procedure wakeup
  }
  if(L < LN) //!< If L < LN
  {
    changestat(j, "branch");
    
    std::vector<std::string> m; 
    m.push_back("initiate"); //!< Initiate()
    m.push_back(std::to_string(LN)); //!< LN
    m.push_back(FN); //!< FN
    m.push_back(SN); //!< SN

    sendMessage(j, msgCreater(m)); //!< send Initiate(LN, FN, SN) on edge j 
    
    if(SN == "find") //!< if SN = Find then
    {
      find_count++; //!< find-count <- find-count +1
    }
    
    free(msg);
    msg = NULL;
  }
  else if(SE[j] == "basic") //!< else if SE(j) = Basic
  {
    sendMessage(nodeid, msg); //!< place receieved message on end of queue
  }
  else
  {
    std::vector<std::string> st; 
    st.push_back("initiate"); //!< Initiate()
    st.push_back(std::to_string(LN+1)); //!< LN +1
    st.push_back(std::to_string(nbd[j]));//!< w(j)
    st.push_back("find");//!< Find
    sendMessage(j, msgCreater(st)); //! send Initiate(LN+1, w(j), Find) on edge j
    free(msg);
    msg = NULL;
  }
}

void GHSNode::handleInitiate()
{
  std::vector<std::string> mg = msg->getMessage();
  int j = std::stoi(mg[0]); //!< edge j
  int L = std::stoi(mg[2]); //!< L
  std::string F = mg[3]; //!< F
  std::string S = mg[4]; //!< S
  LN = L; //!< LN <- L
  FN = F; //!< FN <- F
  SN = S; //!< SN <- S
  in_branch = j; //!< in-branch <- j
  best_edge = -1; //!< best-edge <- nil
  best_weight = INF; //!< best-wt <- infinity
  for(auto it : branch) //!< forall
  {
    if(it.second == j)
    {
      continue;
    }//!< i != j and SE(i) = branch
    std::vector<std::string> st; 
    st.push_back("initiate"); //!< Initiate()
    st.push_back(std::to_string(L)); //!< L
    st.push_back(F); //!< F
    st.push_back(S); //!< S
    sendMessage(it.second, msgCreater(st)); //!< send Initiate(L, F, S) on edge i
    if(S == "find") //!< if S = Find 
    {
      find_count++; //!< find-count <- find-count + 1
    }
  }
  if(S == "find") //!< if S = Find
  {
    test(); //!< execute procedure test
  }
  free(msg);
  msg = NULL;
}

void GHSNode::handleTest()
{
  std::vector<std::string> mg = msg->getMessage();
  int j = std::stoi(mg[0]); //!< edge j 
  if(SN == "sleep") //!< If SN = Sleeping 
  {
    wakeup(); //!< execute procedure wakeup
  }
  int L = std::stoi(mg[2]); //!< L
  std::string F = mg[3]; //!< F
  if(L > LN) //!< L > LN
  {
    sendMessage(nodeid, msg); //!< place recieved message on end of queue
  }
  else if(F != FN) //!< F != FN
  {
    std::vector<std::string> st;
    st.push_back("accept"); //!< Accept()
    sendMessage(j,msgCreater(st)); //!< Send Accept on edge j
    free(msg); 
    msg = NULL;
  }
  else
  {
    if(SE[j] == "basic") //!< if SE(j) = Basic
    {
      changestat(j, "reject");
    }
    if(test_edge != j) //!< test-edge != j
    {
      std::vector<std::string> st;
      st.push_back("reject"); //!< Reject()
      sendMessage(j, msgCreater(st)); //!< send Reject on edge j
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
  std::vector<std::string> mg = msg->getMessage();
  int j = std::stoi(mg[0]); //!< edge j
  test_edge = -1; //!< test-edge <- nil
  if(nbd[j] < best_weight) //!< w(j) < best-wt 
  {
    best_edge = j; //!< best-edge <- j
    best_weight = nbd[j]; //!< best-wt <- w(j)
  }
  free(msg);
  msg = NULL;
  report(); //!< execute procedure report()
}

void GHSNode::handleReject()
{
  std::vector<std::string> mg = msg->getMessage(); 
  int j = std::stoi(mg[0]); //!< edge j
  if(SE[j] == "basic") //!< if SE(j) = Basic 
  {
    changestat(j, "reject");
  }
  free(msg);
  msg = NULL;
  test(); //!< execute procedure test()
}

void GHSNode::handleReport()
{
  std::vector<std::string> mg = msg->getMessage();
  int j = std::stoi(mg[0]); //!< edge j
  int w = std::stoi(mg[2]); //!< w
  if(j != in_branch) //!< if j != in-branch
  {
    find_count--; //!< find-count <- find-count - 1
    if(w < best_weight) //!< w < best-wt
    {
      best_weight = w; //!< best-wt <- w
      best_edge = j; //!< best-edge <- j
    }
    free(msg);
    msg = NULL;
    report(); //!< execute procedure report()
  }
  else if(SN == "find") //!< SN = Find
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
  if(SE[best_edge] == "branch") //!< SE(best-edge) = Branch
  {
    std::vector<std::string> st;
    st.push_back("changeroot"); //!< Change-root()
    sendMessage(best_edge, msgCreater(st)); //!< send Change-root() on best-edge
  }
  else
  {
    std::vector<std::string> st;
    st.push_back("connect"); //!< Connect()
    st.push_back(std::to_string(LN)); //!< LN
    sendMessage(best_edge, msgCreater(st)); //!< send Connect(LN) on best-edge
    changestat(best_edge, "branch");
  }
}

void GHSNode::report()
{
  if(find_count == 0 && test_edge == -1) //!< if find-count = 0 and test-edge = nil
  {
    SN = "found"; //!< SN <- Found
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
    st.push_back(std::to_string(LN)); //!< LN
    st.push_back(FN); //!< FN
    sendMessage(test_edge, msgCreater(st)); //!< send Test(LN, FN) on test-edge
  }
  else //!< Else test-edge <- nil (by default)
  {
    report(); //!< execute procedure report()
  }
}

void GHSNode::sentmessagePrinter(int dest, Message *msg)
{
  printNode("SEN");
  if(msg == prev_msg)
  {
    return;
  }
  else
  {
    prev_msg = msg;
  }
  ofs << "\nSENDING MESSGAE\n" << std::endl;
  if(msg == NULL)
  {
    ofs << "No Valid Message Found" << std::endl;
    return;
  }
  std::string fromnode = std::to_string(nodeid);
  std::vector<std::string> mm = msg->getMessage();
  if(nodeid == dest)
  {
    fromnode = mm[0];
  }
  ofs << "MESSAGE from Node : " << fromnode << std::endl;
  ofs << "MESSAGE to Node : " << dest << std::endl;
  for(int i = 1; i < (int)(mm.size()); i++)
  {
    ofs << "Row " << i << " : " << mm[i] << std::endl;
  }
  ofs << "\nSENT MESSAGE PRINTED\n" << std::endl;
}
  
void GHSNode::recievedmessagePrinter(Message *msg)
{
  printNode("REC");
  if(msg == prev_msg1)
  {
    return;
  }
  else
  {
    prev_msg1 = msg;
  }
  ofs << "\nRECIEVED MESSGAE\n" << std::endl;
  if(msg == NULL)
  {
    ofs << "No Valid Message Found" << std::endl;
    return;
  }
  std::vector<std::string> mm = msg->getMessage();
  ofs << "MESSAGE from Node : " << mm[0] << std::endl;
  ofs << "MESSAGE to Node : " << nodeid << std::endl;
  for(int i = 1; i < (int)(mm.size()); i++)
  {
    ofs << "Row " << i << " : " << mm[i] << std::endl;
  }
  ofs << "\nRECIEVED MESSAGE PRINTED\n" << std::endl;
}

void GHSNode::runner()
{
  while(!(isc->complete))
  {
    std::cerr << "Node with nodeid : " << nodeid << " waiting for a message from queue at " << nodequeue << std::endl;
    while(!recieveMessage())
    {
      continue;
    }
    std::cerr << "Node with nodeid : " << nodeid << " recieved a message." << std::endl;
    std::vector<std::string> mm = msg->getMessage();
    if(mm.size() < 2)
    {
      if(mm.size() == 0)
      {
        std::cerr << "Completely Empty Message Sent" << std::endl;
      }
      else
      {
        std::cerr << "Empty Message Sent from node : " << mm[0] << std::endl;
      }
      exit(46);
    }
    
    std::string mval = mm[1];
    std::cerr << "Node with nodeid : " << nodeid << " recieved a message from : " << mm[0] << " of type : " << mm[1] << std::endl;
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
      std::cerr << "Message recieved at node : " << nodeid << " from node : " << mm[0] << " is invalid with type : " << mval << std::endl;
    }
  }
}

void GHSNode::printNode(std::string id)
{
  ofs << "PRINTING requested by process : " << id << std::endl << std::endl;
  ofs << "Generic Information : " << std::endl << std::endl;
  ofs << "Nodeid : " << nodeid << std::endl;
  ofs << "State : " << SN << std::endl;
  ofs << "Name : " << FN << std::endl;
  ofs << "Level : " << LN << std::endl;

  ofs << "\nEdge States : " << std::endl << std::endl;

  for(auto it : SE)
  {
    ofs << "Neighbor id : " << it.first << " | Edge State : " << it.second << std::endl;
  }

  ofs << "\nNode Printing ENDED" << std::endl << std::endl;
}


void GHSNode::run()
{
  std::cerr << "Started run() on node with id : " << nodeid << std::endl;
  wakeup();
  std::cerr << "Starting runner() on node with id : " << nodeid << std::endl;
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
  this->nbd = neighbors;
  this->network = net;
  this->ofs.open((std::to_string(nid) + ".txt").c_str());
  this->SN = "sleep";
  this->isc = iscom;
  this->nodequeue = net->getQueue(nid);
  std::cerr << "Queue for nodeid : " << nid << " is at address : " << nodequeue << std::endl;
}

