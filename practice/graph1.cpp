#include <iostream>
#include <boost/graph/undirected_graph.hpp>

using namespace std;
using namespace boost; 

int main( ) { 
  undirected_graph<> g;
  undirected_graph<>::vertex_descriptor u = g.add_vertex();
  undirected_graph<>::vertex_descriptor v = g.add_vertex();
  undirected_graph<>::vertex_descriptor w = g.add_vertex();
  undirected_graph<>::vertex_descriptor x = g.add_vertex();
  add_edge(u, v, g); add_edge(u, w, g); add_edge(u, x, g);

  cout << "Degree of u: " << degree(u, g);
  return 0; 
}
