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

Graph<int, int>* GraphInput()
{
  
  //std::cout << "Reading INPUT" << std::endl;
  
  int n;
  std::cin >> n;
  std::cin.ignore();

  //std::cout << "Number of nodes = " << n << std::endl;

  std::vector<std::tuple<int, int, int> > edges;
  std::string s;
  while(getline(std::cin, s))
  {
    if(s == "")
    {
      break;
    }
    std::vector<int> entry = int_extractor(s.substr(1, s.size()-2));
    
    if(entry.size() != 3)
    {
      std::cerr << "Bad Edge Entry" << std::endl;
      exit(-1);
    }

    edges.push_back(std::make_tuple(entry[0], entry[1], entry[2]));
  }

  Graph<int, int> *graph_object = new Graph<int, int>(n,edges.size(),edges);
  return graph_object;
}

/** @brief Takes Input Graph and starts all the node threads
 */

int main()
{
  Graph<int, int> *input_graph = GraphInput();
  Graph<int, int> *mst = input_graph->MST_Kruskal();
  mst->PrintGraph();
  std::ofstream ofs;
  ofs.open("dotTarget.dot");
  input_graph->DrawGraph(ofs);
  ofs.close();
}
