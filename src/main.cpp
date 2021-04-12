/** @file main.cpp
 * @brief This file contains the "main" function and does I/O and runs the GHS
 * Algorithm
 */

#include<bits/stdc++.h>
#include<pthread.h>
#include "GHSNode.h"

/** @brief Provides Hashing for pair. 
 * Gives a Hash of Two objects of arbitrary type by using XOR.
 */

/******************************* Utility Functions ***************************/

/** @brief Given a comma seperated string, this returns a vector of integers
 * @param s Comma seperated string consisting of 3 integers : 2 vertices and 1 
 * edge
 * @return extracts (from node, to node, edge) from string input
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


/******************************* IO Functions ********************************/

/** @brief Take in the graph as per the assignment statement
 * @param n Number of nodes
 * @param m Number of edges
 * @param edges List of weighted edges
 */

void GraphInput(int &n, int &m, std::vector<std::tuple<int, int, int> > &edges)
{
  std::cin >> n;
  std::cin.ignore();

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
}


/** @brief Breaks down the input into adjacency list
 * @param edges List of edges with their weights
 * @param adj_list Gives adjacency list for every node
 * @param mp Maps the unique edges to the inputs given
 */

void ThreadAdjList(std::vector<std::tuple<int, int, int> > &edges, std::map<int, std::unordered_map<int, int> > &adj_list, std::unordered_map<int, std::pair<int, int> > &mp)
{
  for(auto it : edges)
  {
    int node1 = std::get<0>(it);
    int node2 = std::get<1>(it);
    int weight = std::get<2>(it);
    (adj_list[node1])[node2] = weight;
    (adj_list[node2])[node1] = weight;
    mp[weight] = std::make_pair(node1, node2);
  }
  return;
}

/******************************* GHS Runner **********************************/


/** @brief Helper Function to start instances of GHSNodes
 */

void* run_thread(void * node)
{ 
  ((GHSNode *)node)->run();
  pthread_exit(NULL);
}


/** @brief Starts all GHSNodes on different threads, passes the Final MST Back
 * @param adj_list Adjacency list of the graph
 * @param mp Mapping from weights to node pairs
 * @result Set of edges in MST
 */

std::set<std::tuple <int, int, int> > thread_runner(std::map<int, std::unordered_map<int, int> > &adj_list, std::unordered_map<int, std::pair<int, int> > &mp)
{
  int n = adj_list.size(); //!< Number of Nodes

  std::vector<int> no; //!< Set of nodes 

  for(auto it : adj_list)
  {
    no.push_back(it.first); 
  }

  Network *network = new Network(no); //!< Networks for the threads
  IsComplete *isc = new IsComplete(); //!< Flag to check completion
  TotMessage *tot = new TotMessage(); //!< Total message counter

  std::vector<pthread_t> threads(n); //!< Vector of threads
  std::vector<GHSNode *> nodes; //!< Vector of all GHSNodes
  
  int i = 0;
  for(auto it : adj_list)
  {
    GHSNode *temp = new GHSNode(it.first, it.second , network, isc, tot); //!< Create new GHSNode
    nodes.push_back(temp); 
    
    int errcode = pthread_create(&(threads[i]), NULL, run_thread, (void *)temp); //!< Start the thread, if errcode != 0 then thread creation was not successful
    
    if(errcode != 0)
    {
      std::cerr << "Thread Creation at index : " << i << " Failed.\n Exiting.... " << std::endl;
      exit(49);
    }
    i++;
  }
  
  while(!(isc->complete))
  {
    //threads still running
    continue;
  }
  //GHS Complete
  
  std::cout << tot->getTot() << std::endl;

  std::set<std::tuple <int, int, int > > ans; //!< MST Edges
  for(auto it : nodes)
  {
    std::vector<int> v = it-> getMSTEdges();
    for(auto jt : v)
    {
      ans.insert(std::make_tuple(jt, mp[jt].first, mp[jt].second));
    }
  }
  return ans;
}

/** @brief Prints output MST in requisite form
 * @param out output MST edges
 */

void PrintOutput(std::set<std::tuple<int, int, int> >& out)
{
  for(auto it : out)
  {
    int node1 = std::get<1>(it);
    int node2 = std::get<2>(it);
    int weight = std::get<0>(it);
    std::cout << "(" << node1 << " , " << node2 << ", " << weight << ")" << std::endl; 
  }
}


/******************************* Main Function ********************************/

int main()
{

  auto start = std::chrono::high_resolution_clock::now();
  
  /********************* Initialization ************************************/
  
  int n = -1,m = -1;
  std::vector<std::tuple<int, int, int> > edges;
  std::map<int, std::unordered_map<int, int> > adj_list;
  std::unordered_map<int, std::pair<int, int > > mp;

  /******************** I/O ************************************************/


  GraphInput(n,m,edges);

  if(n < 2){exit(0);}

  /******************** FormatChanges **************************************/

  ThreadAdjList(edges, adj_list, mp);

  /******************** Run GHS ********************************************/
  
  std::set<std::tuple<int, int, int> > out = thread_runner(adj_list, mp);

  //PrintOutput(out);

  auto end = std::chrono::high_resolution_clock::now();

  auto timeval = (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count();
  
  //std::cout << ((double)timeval)/1000000.0 << std::endl;

}
