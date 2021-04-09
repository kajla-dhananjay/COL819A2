#include<bits/stdc++.h>

/** @file dot_graph.h
 * Header file for dot_graph
 */

#ifndef _DOT_GRAPH_H_
#define _DOT_GRAPH_H_

/** @brief The class plots the graph
 * @param ofs File Stream to write the graph dotfile into
 * @param edges List of edges of the graph 
 * @param labels Weights of the corresponding edges
 * @param root Root of the graph
 * @param has_labels Flag to check whether graph is weighted
 * @param is_directed Flag to check whether the graph is directed.
 */


template<typename T, typename U>class GraphVz
{
  private:
  public:
    GraphVz(std::ofstream& ofs, const std::vector<std::pair<T, T>>& edges, const std::vector<U>& labels, T root, bool has_labels = false, bool is_directed = false);
};

#endif
