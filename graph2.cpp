#include <iostream>
#include <boost/graph/adjacency_list.hpp> 

using namespace std;
using namespace boost;
typedef boost::adjacency_list<listS, vecS, bidirectionalS> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::vertex_iterator VertexI;
typedef Graph::in_edge_iterator inEdgeI;
typedef Graph::out_edge_iterator outEdgeI;

int main() { 
  Graph g;
  Vertex u = add_vertex(g);
  Vertex v = add_vertex(g);
  Vertex w = add_vertex(g);
  Vertex x = add_vertex(g);
  add_edge(u, v, g); add_edge(u, w, g); add_edge(u, x, g);
  cout << "Number of edges: " << num_edges(g) << "\n";
  cout << "Number of vertices: " << num_vertices(g) << "\n";
  VertexI vertexIt, vertexEnd;
  inEdgeI inedgeIt, inedgeEnd;
  outEdgeI outedgeIt, outedgeEnd;
  tie(vertexIt, vertexEnd) = vertices(g);
  for (; vertexIt != vertexEnd; ++vertexIt) {
    cout << "incoming edges for " << *vertexIt << ": ";
    tie(inedgeIt, inedgeEnd) = in_edges(*vertexIt, g); 
    for(; inedgeIt != inedgeEnd; ++inedgeIt) { 
      cout << *inedgeIt << " "; 
    }
    cout << "; in degree: " << in_degree(*vertexIt, g) << "\n"; 
    cout << "outgoing edges for " << *vertexIt << ": ";
    tie(outedgeIt, outedgeEnd) = out_edges(*vertexIt, g);
    for(; outedgeIt != outedgeEnd; ++outedgeIt) { 
      cout << *outedgeIt << " "; 
    }
    cout << "; out degree: " << out_degree(*vertexIt, g) << "\n"; 
  }
  return 0; 
}
