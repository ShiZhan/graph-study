#include <iostream>
#include <utility>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;
typedef adjacency_list<listS, vecS, directedS, no_property, property<edge_weight_t, int>> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef pair<int, int> Edge;

int main(int argc, char* argv[])
{
  enum { A, B, C, D, E, N };
  const int num_vertices = N;
  Edge edge_array[] = { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C), Edge(C,E), Edge(B,D), Edge(D,E) };
  const int num_edges = sizeof(edge_array)/sizeof(Edge);
  int weights[] = { 1, 2, 1, 2, 7, 3, 1};
  Graph g(edge_array, edge_array + num_vertices, weights, num_vertices);
  cout << "edges(g) = ";
  Graph::edge_iterator EdgeI, EdgeLast;
  for (tie(EdgeI, EdgeLast) = edges(g); EdgeI != EdgeLast; ++EdgeI)
    cout << "(" << source(*EdgeI, g) << "," << target(*EdgeI, g) << ") ";
  cout << endl;
  vector<int> distances(num_vertices);
  Vertex start = *(vertices(g).first);
  dijkstra_shortest_paths(g, start, distance_map(&distances[0]));
  cout << "distances from start vertex:" << endl;
  Graph::vertex_iterator VertexI, VertexLast;
  
  for(tie(VertexI, VertexLast) = vertices(g); VertexI != VertexLast; ++VertexI)
    cout << "distance(" << *VertexI << ") = " << distances[*VertexI] << endl;
  cout << endl;
  return 0;
}
