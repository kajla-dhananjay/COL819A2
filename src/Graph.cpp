/** @file Graph.cpp
 * Provides Implementation of the Graph Class.  */

#include<bits/stdc++.h>
#include "dot_graph.h"
#include "Graph.h"


GraphException::GraphException()
{
  std::cout << "Exception encountered in GraphVz library" << std::endl;
  return;
}

GraphException::GraphException(int code)
{
  switch(code)
  { case 0: { std::cout << "File is not open" ;break; } case 1:
      {
        std::cout << "Incorrect Labels provided" << std::endl;break;
      }
    case 2:
      {
        std::cout << "Number of edges don't match number of labels" << std::endl;break;
      }
    case 3:
      {
        std::cout << "Self loop detected" << std::endl;break;
      }
    case 4:
      {
        std::cout << "Incorrect Number of Nodes Provided" << std::endl;break;
      }
    default:{std::cout << "Exception encountered in GraphVz library with code : " << code << std::endl; break;}
  }
  exit(code);
}


template<typename T, typename U>
Graph<T, U>::Graph(int n, int m, std::vector<std::tuple<T,T,U> > weight_labels)
{
  num_nodes = n;
  num_edges = m;
  adjacency_matrix.resize(n, std::vector<U>(n));
  adjacency_list.resize(n);


  if(m != weight_labels.size())
  {
    throw GraphException(2);
  }

  for(auto it : weight_labels)
  {

    T node1         = std::get<0>(it);
    T node2         = std::get<1>(it);
    int node1_index = -1        ;
    int node2_index = -1        ;
    U edge_label    = std::get<2>(it);
    
    if(node1 == node2)
    {
      throw GraphException(3);
    }
    
    if(nodes.find(node1) == nodes.end())
    {
      nodes.insert(node1);
      node_indices[node1] = node_labels.size();
      node_labels.push_back(node1);
    }

    if(nodes.find(node2) == nodes.end())
    {
      nodes.insert(node2);
      node_indices[node2] = node_labels.size();
      node_labels.push_back(node2);
    }
    
    if(nodes.size() > n)
    {
      throw GraphException(4);
    }

    node1_index = node_indices[node1];
    node2_index = node_indices[node2];
    
    edge_set.insert(std::make_tuple(edge_label, node1_index, node2_index));
    edges_reverse_hashmap[edge_label] = std::make_pair(node1, node2);
    edges_hashmap[std::make_pair(node1, node2)] = edge_label;
    edges_vector.push_back(std::make_pair(node1, node2));
    weight_vector.push_back(edge_label);
    adjacency_matrix[node1_index][node2_index] = edge_label;
    adjacency_list[node1_index][node2_index] = edge_label;
  
  }

}

template<typename T, typename U>
void Graph<T, U>::DrawGraph(std::ofstream &ofs)
{
  GraphVz<T,U> gobject(ofs, edges_vector, weight_vector, -1, true, false);
}

template<typename T, typename U>
void Graph<T, U>::PrintGraph()
{
  std::cout << "\n\nGraph Printing Start : " << std::endl;
  std::cout << "Number of Nodes : " << num_nodes << std::endl;
  std::cout << "Number of Edges : " << num_edges << std::endl;
  
  std::cout << "\nEdge Vector : \n" << std::endl;
  for(int i = 0; i < num_edges; i++)
  {
    std::cout << "EDGE START\n" << std::endl;
    std::cout << "First Node Label : " << edges_vector[i].first << " | First Node Index : " << node_indices[edges_vector[i].first] << std::endl;
    std::cout << "Second Node Label : " << edges_vector[i].second << " | Second Node Index : " << node_indices[edges_vector[i].second] << std::endl;
    std::cout << "Edge Weight : " << weight_vector[i] << std::endl << std::endl;
    std::cout << "Edge END\n" << std::endl;
  }
  
  std::cout << "\nLabel to Index\n" << std::endl;
  for(auto it : node_indices)
  {
    std::cout << "Node Label = " << it.first << " | " << "Node Index = " << it.second << std::endl;
  }
  
  std::cout << "\nIndex to Label\n" << std::endl;
  for(size_t i = 0; i < node_labels.size(); i++)
  {
    std::cout << "Node Index = " << i << " | Node Label = " << node_labels[i] << std::endl;
  }
 
  std::cout << "\nEdges sorted by ascending order of weight\n" << std::endl;
  for(auto it : edge_set)
  {
    std::cout << "EDGE START\n" << std::endl;
    std::cout << "First Node Label : " << node_labels[std::get<1>(it)] << " | First Node Index : " << std::get<1>(it) << std::endl;
    std::cout << "Second Node Label : " << node_labels[std::get<2>(it)] << " | Second Node Index : " << std::get<2>(it) << std::endl; 
    std::cout << "Edge Weight : " << std::get<0>(it) << std::endl;
    std::cout << "\nEdge END\n" << std::endl;
  }

  std::cout << "\nAdjacency Matrix\n" << std::endl;
  for(size_t i = 0; i < adjacency_matrix.size(); i++)
  {
    std::cout << "For node " << i << " : ";
    for(size_t j = 0; j < adjacency_matrix[i].size(); j++)
    {
      std::cout << adjacency_matrix[i][j] << ' ';
    }
    std::cout << std::endl;
  }

  std::cout << "\nAdjacency List\n" << std::endl;
  for(size_t i = 0; i < adjacency_list.size(); i++)
  {
    for(auto it : adjacency_list[i])
    {
      std::cout << i << " " << it.first << " " << it.second << std::endl;
    }
  }
  std::cout << "\nGraph Output\n" << std::endl;
  PrintOutput();

  std::cout << "\n\nGraph Printing End" << std::endl;
}

template<typename T, typename U>
void Graph<T, U>::PrintOutput()
{
  for(auto it : edge_set)
  {
    std::cout << "( " << node_labels[std::get<1>(it)] << " , " << node_labels[std::get<2>(it)] << " , " << std::get<0>(it) << " )" << std::endl;
  }
}

template<typename T, typename U>
bool Graph<T, U>::IsConnected()
{
  std::queue<int> node_queue;
  node_queue.push(0);
  std::set<int> visited;
  visited.insert(0);
  while(!node_queue.empty())
  {
    int nodev = node_queue.front();
    node_queue.pop();
    visited.insert(nodev);
    for(auto it : adjacency_list[nodev])
    {
      if(visited.find(it.first) == visited.end())
      {
        node_queue.push(it.first);
      }
    }
  }
  return (visited.size() == num_nodes);
}

template<typename T, typename U>
Graph<T, U>* Graph<T, U>::MST_Kruskal()
{
  std::vector<int> color(num_nodes,-1);
  for(int i = 0; i < num_nodes ; i++)
  {
    color[i] = i;
  }
  int total_cost = 0;
  std::vector<std::tuple<T, T, U> > mst_edges;
  for(auto it : edge_set)
  {
    if(color[std::get<1>(it)] != color[std::get<2>(it)])
    {
      total_cost+= std::get<0>(it);
      mst_edges.push_back(std::make_tuple(node_labels[std::get<1>(it)], node_labels[std::get<2>(it)], std::get<0>(it)));
      int temp = color[std::get<1>(it)];
      int ncol = color[std::get<2>(it)];
      for(auto &it : color)
      {
        if(it == temp)
        {
          it = ncol;
        }
      }
    }
  }
  Graph<T, U>* mst = new Graph<T, U>(num_nodes, mst_edges.size(), mst_edges);
  return mst;
}

template class Graph<int, int>;
#ifdef STANDALONE
int main()
{
  Graph<int, int> *newGraph = new Graph<int, int>(2,1,{std::make_tuple(2,1,3)});
  newGraph->PrintGraph();
}
#endif
