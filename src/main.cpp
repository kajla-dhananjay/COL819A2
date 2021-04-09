/** @file main.cpp
 * @brief This file contains the "main" function and does I/O and runs the GHS Algorithm
 */

#include<bits/stdc++.h>
#include "dot_graph.h"
#include "Graph.h"


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

  //std::cout << "Number of nodes = " << n << std::endl;

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

  Graph<int, int> *graph_object = new Graph<int, int>(n,edges.size(),edges);
  return graph_object;
}

//index -> current node
//it.first -> neighbor
//it.second -> weight of neighbor
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

Graph<int, int> *thread_runner(std::vector<std::unordered_map<int, int> > &adj_list)
{
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
  // Define operator == by same edge
  if(mst_ghs == mst_kru)
  {
    std::cerr << "PASS" << std::endl;
    exit(0);
  }
  else
  {
    std::cerr << "FAIL" << std::endl;
    exit(127);
  }
  //mst->PrintOutput();
  //std::ofstream ofmst;
  //ofmst.open("mst.dot");
  //mst->DrawGraph(ofmst);
  //ofmst.close();
  //std::ofstream ofs;
  //ofs.open("input_graph.dot");
  //input_graph->DrawGraph(ofs);
  //ofs.close();
}
