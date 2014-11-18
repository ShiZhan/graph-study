#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;
using namespace boost;
typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<listS, vecS, undirectedS, no_property, EdgeWeightProperty> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;

int main() {
  Graph g;
  Vertex u = add_vertex(g);
  Vertex v = add_vertex(g);
  Vertex w = add_vertex(g);
  Vertex x = add_vertex(g);
  add_edge(u, v, 10, g);
  add_edge(u, w,  5, g);
  add_edge(u, x,  3, g);
  add_edge(v, w,  1, g);
  add_edge(v, x,  3, g);
  add_edge(w, x,  7, g);
  cout << "Number of edges: " << num_edges(g) << "\n";
  cout << "Number of vertices: " << num_vertices(g) << "\n";
  list<Edge> spanning_tree;
  kruskal_minimum_spanning_tree(g, back_inserter(spanning_tree));
  for (list<Edge>::iterator ei = spanning_tree.begin(); ei != spanning_tree.end(); ++ei) {
    cout << *ei << " ";
  }
  cout << "\n";
  return 0;
}
