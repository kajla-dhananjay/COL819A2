/** @file main.cpp
 * @brief This file contains the "main" function and does I/O and runs the GHS Algorithm
 */

#include<bits/stdc++.h>
#include<pthread.h>
#include "dot_graph.h"
#include "Graph.h"
#include "GHSNode.h"

/** @brief Given a comma seperated string, this returns a vector of integers
 * @param s Comma seperated string consisting of 3 integers : 2 vertices and 1 edge 
 */

std::vector<int> int_extractor(std::string s)
{
  std::vector<int> v;
  std::string temp = "";
  for(char it : s)
  {
    if('0' <= it && it <= '9')
    {
      temp += it;
      continue;
    }
    if(temp == "")
    {
      continue;
    }
    int z = std::stoi(temp);
    v.push_back(z);
    temp = "";
  }
  if(temp != "")
  {
    int z = std::stoi(temp);
    v.push_back(z);
  }
  return v;
}


/** @brief Take in the graph as per the assignment statement
 */

Graph<int, int>* GraphInput(int &n, int &m, std::vector<std::tuple<int, int, int> > &edges)
{
  
  //std::cout << "Reading INPUT" << std::endl;
  
  std::cin >> n;
  std::cin.ignore();

#ifdef Debug
  std::cerr << "Number of nodes = " << n << std::endl;
#endif

  std::string s;
  while(getline(std::cin, s))
  {
    if(s == "")
    {
      break;
    }
    std::vector<int> entry = int_extractor(s);
    
    if(entry.size() != 3)
    {
      std::cerr << "Bad Edge Entry" << std::endl;
      exit(-1);
    }

    edges.push_back(std::make_tuple(entry[0], entry[1], entry[2]));
  }
  
  m = edges.size();
  
#ifdef Debug
  std::cerr << "Number of edges = " << m << std::endl;
#endif 

  Graph<int, int> *graph_object = new Graph<int, int>(n,edges.size(),edges);
  return graph_object;
}

/** @brief Breaks down the input into adjacency list
 * @param n Number of nodes
 * @param m Number of edges
 * @param edges List of edges with their weights
 */

std::vector<std::unordered_map<int, int> > ThreadAdjList(int &n, int &m, std::vector<std::tuple<int, int, int> > &edges)
{
  std::vector<std::unordered_map<int, int> > adj_list(n);
  for(auto it : edges)
  {
    int node1 = std::get<0>(it);
    int node2 = std::get<1>(it);
    int weight = std::get<2>(it);
    adj_list[node1][node2] = weight;
    adj_list[node2][node1] = weight;
  }
  return adj_list;
}

/** @brief Helper Function to start instances of GHSNodes
 */

void* run_thread(void * node)
{ 
  ((GHSNode *)node)->run();
  pthread_exit(NULL);
}


/** @brief Starts all GHSNodes on different threads, passes the Final MST Back
 */

Graph<int, int> *thread_runner(std::vector<std::unordered_map<int, int> > &adj_list)
{
  int n = adj_list.size(); //!< Number of Nodes

  Network *network = new Network();

  std::vector<pthread_t> threads(n); //!< Vector of threads
  std::vector<GHSNode *> nodes; //!< Vector of all GHSNodes

  for(int i = 0; i < n; i++)
  {
    GHSNode *temp = new GHSNode(i,adj_list[i],network); //!< Create new GHSNode
    nodes.push_back(temp);
    // Flag : Check for failure before submission
    int errcode = pthread_create(&(threads[i]), NULL, run_thread, (void *)temp); //!< Start the thread, if errcode != 0 then thread creation was not successful
    if(errcode != 0)
    {
      std::cerr << "Thread Creation at index : " << i << " Failed.\n Exiting.... " << std::endl;
      exit(49);
    }
    //std::cout << "Node with index : " << i << " is running on thread with id : " << threads[i] << std::endl;
  }
  
  std::vector<void *> exitStat(n);

  for(int i = 0; i < n; i++)
  {
    pthread_join((threads[i]),&(exitStat[i]));
  }
  //t.join 
  // Look at the hasMst variable for all nodes
  // Return MST of the node which has mst

  return NULL;

  // Initializes all nodes
  // ith node will get adj_list[i] (Neighborhood set)
  // Once all threads end, we get MST from one of the threads in format F.
  // Convert F to Graph<int, int>
  // return this graph
}

int main()
{
  int n = -1,m = -1;
  std::vector<std::tuple<int, int, int> > edges;
  
  Graph<int, int> *input_graph = GraphInput(n,m,edges);
  std::vector<std::unordered_map<int, int> > adj_list = ThreadAdjList(n,m,edges);
  
  Graph<int, int> *mst_ghs = thread_runner(adj_list);
  
  Graph<int, int> *mst_kru = input_graph->MST_Kruskal();
  
  if(mst_ghs == mst_kru)
  {
    std::cerr << "PASS" << std::endl;
  }
  else
  {
    std::cerr << "FAIL" << std::endl;
  }
  



  //std::ofstream ofmst;
  //ofmst.open("mst.dot");
  //mst_kru->DrawGraph(ofmst);
  //ofmst.close();
  //std::ofstream ofs;
  //ofs.open("input_graph.dot");
  //input_graph->PrintGraph();
  //input_graph->DrawGraph(ofs);
  //ofs.close();
}
