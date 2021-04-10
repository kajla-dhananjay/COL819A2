#include <bits/stdc++.h>
#include "Graph.h"
#include "GHSNode.h"

GHSNode::GHSNode(int nid, std::unordered_map<int, int> neighbors)
{
  this->nodeid = nid;
  this->nbd_list = neighbors;
}

Graph<int, int> * GHSNode::run()
{
  initialize();
  return runner();
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

Message GHSNode::msgCreater(std::vector<std::string> msg)
{
  std::vector<std::string> m;
  m.push_back(std::to_string(nodeid));
  for(auto it : msg)
  {
    m.push_back(it);
  }
  return m;
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

Graph<int, int> * GHSNode::runner()
{
  //while(Queue is empty)
  //{
  //  wait
  //}
  //DoSomething();
}

void GHSNode::sendMessage(int dest, Message m)
{
  network->msg_queues[dest].push(m);
}
