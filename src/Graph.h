/** @file Graph.h
 * Provides Signature for the Graph Class.
 */

#include<bits/stdc++.h>
#include "dot_graph.h"

#ifndef _GRAPH_H_
#define _GRAPH_H_
/** @brief Provides Hashing for pair. 
 * Gives a Hash of Two objects of arbitrary type by using XOR.
 */

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class GraphException
{
  public:
    GraphException();
    GraphException(int code);
}; 

/** @brief Stores Undirected Weighted Graphs.
 * Provides Undirected Weighted Graph ADT and provides some graph probabilities
 */

template<typename T, typename U>
class Graph{
  private:
    int num_nodes; //!< Number of nodes
    int num_edges; //!< Number of edges
    std::set<T> nodes; //!< Set of node labels
    std::vector<T> node_labels; //!< Stores the mapping of node indices to node labels
    std::unordered_map<T, int> node_indices; //!< Stores the mapping of node labels to node indices
    std::set<std::tuple<U, T, T> > edge_set; //!< Stores Edges in a sorted order
    std::unordered_map<std::pair<T,T>, U, hash_pair> edges_hashmap; //!< Stores the mapping from a pair of node labels to edge labels
    std::unordered_map<U, std::pair<T,T> > edges_reverse_hashmap; //!< Gives the adjacent pair of nodes for given edge 
    std::vector<std::pair<T,T> > edges_vector; //!< List of edges as a pair of nodes
    std::vector<U> weight_vector; //!< List of weights of edges
    //std::vector<std::vector<U> > adjacency_matrix; //!< Adjacency matrix from node indices to Edge labels
    std::vector<std::unordered_map<int, U> > adjacency_list; //!< Adjacency list of node indices
  public:
    bool operator == (Graph &obj);
    Graph(int n, int m, std::vector<std::tuple<T, T, U> > weights_labels); //!< Graph Constructor to take in the graph in given format
    std::set<std::tuple<U,T,T> > GetEdgeSet();
    void DrawGraph(std::ofstream &ofs); //!< Puts the graph into ofs file
    void PrintGraph(); //!< Prints The various data structures of the graph
    void PrintOutput(); //!< Prints The output as requested
    bool IsConnected(); //!< Checks If the graph is connected
    Graph<T, U>* MST_Kruskal(); //!< Gives the MST for the given graph
};
#endif
