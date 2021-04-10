/** @file dot_graph.cpp
 * @brief Uses GraphViz Library to plot Graphs
 */

#include <bits/stdc++.h>
#include "dot_graph.h"
#include "Graph.h"
//#define STANDALONE

/** @brief Constructor for taking in parameters of the graph and file
 * @param ofs File Stream to write the graph dotfile into
 * @param edges List of edges of the graph 
 * @param labels Weights of the corresponding edges
 * @param root Root of the graph
 * @param has_labels Flag to check whether graph is weighted
 * @param is_directed Flag to check whether the graph is directed.
 */

template<typename T, typename U>
GraphVz<T,U>::GraphVz(std::ofstream& ofs, const std::vector<std::pair<T, T>>& edges, const std::vector<U>& labels, T root, bool has_labels, bool is_directed)
{
  if(!ofs.is_open())
  {
    throw GraphException(0);
    return;
  }
  if(has_labels && (edges.size() != labels.size()))
  {
    throw GraphException(1);
    return;
  }
  if(is_directed)
  {
    ofs << "digraph G {\n";
  } else
  {
    ofs << "graph G{\n";
  }
  ofs << "graph [dpi = 300, overlap = false];\n";
  ofs << "  nodesep = 20.0;\n";
  ofs << "  edge [color=\"#ff5555\"];\n";
  std::unordered_set<T> st;
  size_t m = edges.size();
  for(size_t i = 0; i < m; i++)
  {
    T from = edges[i].first, to = edges[i].second;
    st.insert(from);
    st.insert(to);
    ofs << " " << from << (is_directed ? " -> " : " -- ") << to;
    ofs << " [style=bold";
    if(has_labels)
    {
      ofs << ",label=\"" << labels[i] << "\"";
    }
    ofs << "];\n";
  }
  bool is_rooted = false;
  for (auto it = st.begin(); it != st.end(); it++) 
  {
    ofs << "  " << *it << " [shape=circle";
    if (root == *it) 
    {
      ofs << ",peripheries=2";
      is_rooted = true;
    }
    ofs << "];\n";
  }
  ofs << "}\n";
}

template class GraphVz<int, int>;
#ifdef STANDALONE 
int main() 
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  // Example usage
  std::vector<std::pair<int, int> > edges;
  std::vector<int> labels;
  int m;
  std::cin >> m;
  for (int i = 0; i < m; i++) 
  {
    std::pair<int, int> ed;
    int la = 0;
    std::cin >> ed.first >> ed.second >> la;
    edges.emplace_back(ed);
    labels.push_back(la);
  }
  std::ofstream ofs;
  ofs.open("DotTarget.dot");
  GraphVz<int, int> gobject(ofs, edges, labels, -1, true, false);
  return 0;
}
#endif
