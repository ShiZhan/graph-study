#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/rmat_graph_generator.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/random/linear_congruential.hpp>
#include "options.h"
#include "utils.h"

using namespace std;
using namespace boost;
typedef adjacency_list<setS, vecS, directedS, no_property> graph_t;
typedef rmat_iterator<minstd_rand, graph_t> rmat_gen;

class custom_bfs_visitor : public default_bfs_visitor {
public:
	template < typename Vertex, typename Graph >
	void discover_vertex(Vertex v, const Graph &g) const {
		cout << v << endl;
	}
};

class custom_dfs_visitor : public default_dfs_visitor {
public:
	template < typename Vertex, typename Graph >
	void discover_vertex(Vertex v, const Graph &g) const {
		cout << v << endl;
	}
};

uint get_edges(char* edges_file, graph_t &g) {
	uint u, v, total = 0;
	string line;
	if (edges_file) {
		ifstream ifs(edges_file);
		if(ifs) {
			while (getline(ifs, line)) {
				SSCANF((line.c_str(), "%u %u", &u, &v));
				add_edge(u, v, g);
				++total;
			}
			ifs.close();
		}
	} else {
		while (getline(cin, line)) {
			SSCANF((line.c_str(), "%u %u", &u, &v));
			add_edge(u, v, g);
			++total;
		}
	}
	return total;
}

#define SOURCE 0
#define V 8
#define E 8

int main(int argc, char* argv[]) {
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "bgl-shell -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -g:\t generate graph with 2^(" << V << ") vertices and (" << E << ") times edges" << endl;
		cout << " -i:\t (cin) input edge list" << endl;
		cout << " -e:\t perform [BFS|DFS], etc." << endl;
		cout << " -s:\t (" << SOURCE << ") specify source vertex" << endl;
		return 0;
	}

	char* rmat_opt   = getOption(argv, argv + argc, "-g");
	bool  use_rmat   = chkOption(argv, argv + argc, "-g");
	char* edges_file = getOption(argv, argv + argc, "-i");
	char* algorithm  = getOption(argv, argv + argc, "-e");
	uint  source     = getInt(argv, argv + argc, "-s", SOURCE);

	graph_t g;
	uint total_vertices = 0, total_edges = 0;
	if (use_rmat) {
		uint scale_v = V, scale_e = E;
		if (rmat_opt) SSCANF((rmat_opt, "%u:%u", &scale_v, &scale_e));
		total_vertices = 1 << scale_v;
		total_edges    = total_vertices * E;
		minstd_rand gen;
		gen.seed((uint)time(NULL));
		g = graph_t(
			rmat_gen(gen, total_vertices, total_edges, 0.57, 0.19, 0.19, 0.05),
			rmat_gen(),
			total_vertices);
	} else {
		uint total_edges    = get_edges(edges_file, g);
		uint total_vertices = num_vertices(g);
	}

	if (algorithm) {
		if (!(strcmp(algorithm, "bfs") && strcmp(algorithm, "BFS"))) {
			custom_bfs_visitor b_v;
			breadth_first_search(g, vertex(source, g), visitor(b_v));
		} else if (!(strcmp(algorithm, "dfs") && strcmp(algorithm, "DFS"))) {
			custom_dfs_visitor d_v;
			depth_first_search(g, root_vertex(vertex(source, g)).visitor(d_v));
		} else { cout << "Available algorithms: BFS, DFS." << endl; }
	} else {
		if (use_rmat) {
			graph_t::edge_iterator ei, ei_end;
			for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
				cout << boost::source(*ei, g) << " " << boost::target(*ei, g) << endl;
		} else {
			print_graph(g);
		}
	}
}
