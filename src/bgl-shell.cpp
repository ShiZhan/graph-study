#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <time.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/rmat_graph_generator.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/graph/small_world_generator.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/random/linear_congruential.hpp>
#include "options.h"
#include "utils.h"

using namespace std;
using namespace boost;

class custom_bfs_visitor : public default_bfs_visitor {
public:
	template <typename Vertex, typename Graph>
	void discover_vertex(Vertex v, const Graph& g) const {
		cout << v << endl;
	}
};

class custom_dfs_visitor : public default_dfs_visitor {
public:
	template <typename Vertex, typename Graph>
	void discover_vertex(Vertex v, const Graph& g) const {
		cout << v << endl;
	}
};

template <class Graph>
uint get_edges(char* edges_file, Graph& g) {
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

template <class Graph>
void print_edges(const Graph& g) {
	BGL_FORALL_EDGES_T(e, g, Graph)
		cout << source(e, g) << " " << target(e, g) << endl;
}

template <class Graph>
bool has_self_loop(const Graph& g) {
	BGL_FORALL_EDGES_T(e, g, Graph)
		if (source(e, g) == target(e, g)) return true;
	return false;
}

template <class Graph>
bool is_dag(const Graph& g) {
	uint total_vertices = num_vertices(g);
	vector<uint> component(total_vertices);
	uint total_components = strong_components(g, make_iterator_property_map(component.begin(), get(vertex_index, g)));
	return (total_components == total_vertices) && !has_self_loop(g);
}

#define DEFAULT_ROOT 0
#define DEFAULT_RMAT "8:8"
#define DEFAULT_ER   "256:0.05"
#define DEFAULT_SW   "256:6:0.03"

int main(int argc, char* argv[]) {
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "bgl-shell -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -g:\t (RMAT) use generator [RMAT|ER|SW]" << endl;
		cout << " -p:\t set graph generator parameters" << endl;
		cout << " \t   Recursive-MATrix: " << DEFAULT_RMAT << endl;
		cout << " \t   Erdos-Renyi:      " << DEFAULT_ER   << endl;
		cout << " \t   Small-World:      " << DEFAULT_SW   << endl;
		cout << " -i:\t (cin) input edge list" << endl;
		cout << " -e:\t perform [BFS|DFS|SCC|TS], etc." << endl;
		cout << " \t   BFS: breadth-first traversal" << endl;
		cout << " \t   DFS: depth-first traversal" << endl;
		cout << " \t   SCC: strongly connected components" << endl;
		cout << " \t   TS:  topological sort (on directed acyclic graph only)" << endl;
		cout << " -r:\t (" << DEFAULT_ROOT << ") specify root vertex for graph traversal" << endl;
		return 0;
	}

	bool  use_gen    = chkOption(argv, argv + argc, "-g");
	char* generator  = getOption(argv, argv + argc, "-g");
	char* gen_param  = getOption(argv, argv + argc, "-p");
	char* edges_file = getOption(argv, argv + argc, "-i");
	char* algorithm  = getOption(argv, argv + argc, "-e");
	uint  v_root     = getInt(argv, argv + argc, "-s", DEFAULT_ROOT);

	typedef adjacency_list<setS, vecS, directedS, no_property> graph_t;

	uint total_vertices = 0, total_edges = 0;
	if (use_gen) {
		boost::minstd_rand gen;
		gen.seed((uint)time(NULL));

		if (!generator || !(strcmp(generator, "rmat") && strcmp(generator, "RMAT"))) {
			typedef rmat_iterator<boost::minstd_rand, graph_t> rmat_gen;
			string param = gen_param ? gen_param : DEFAULT_RMAT;
			uint scale_v, scale_e;
			SSCANF((param.c_str(), "%u:%u", &scale_v, &scale_e));
			total_vertices = 1 << scale_v;
			total_edges    = total_vertices * scale_e;
			auto it       = rmat_gen(gen, total_vertices, total_edges, 0.57, 0.19, 0.19, 0.05);
			auto it_last  = rmat_gen();
			for (;it != it_last; ++it) cout << it->first << " " << it->second << endl;
		} else if (!(strcmp(generator, "er") && strcmp(generator, "ER"))) {
			typedef erdos_renyi_iterator<boost::minstd_rand, graph_t> er_gen;
			string param = gen_param ? gen_param : DEFAULT_ER;
			double probability;
			SSCANF((param.c_str(), "%u:%lf", &total_vertices, &probability));
			auto it       = er_gen(gen, total_vertices, probability);
			auto it_last  = er_gen();
			for (;it != it_last; ++it) cout << it->first << " " << it->second << endl;
		} else if (!(strcmp(generator, "sw") && strcmp(generator, "SW"))) {
			typedef small_world_iterator<boost::minstd_rand, graph_t> sw_gen;
			string param = gen_param ? gen_param : DEFAULT_SW;
			uint knn;
			double probability;
			SSCANF((param.c_str(), "%u:%u:%lf", &total_vertices, &knn, &probability));
			auto it       = sw_gen(gen, total_vertices, knn, probability);
			auto it_last  = sw_gen();
			for (;it != it_last; ++it) cout << it->first << " " << it->second << endl;
		} else {
			cout << "Available generators: RMAT, ER, SW." << endl;
		}
	} else {
		graph_t g;
		total_edges = get_edges(edges_file, g);

		if (algorithm) {
			if (!(strcmp(algorithm, "bfs") && strcmp(algorithm, "BFS"))) {
				custom_bfs_visitor b_v;
				breadth_first_search(g, vertex(v_root, g), visitor(b_v));
			} else if (!(strcmp(algorithm, "dfs") && strcmp(algorithm, "DFS"))) {
				custom_dfs_visitor d_v;
				depth_first_search(g, root_vertex(vertex(v_root, g)).visitor(d_v));
			} else if (!(strcmp(algorithm, "scc") && strcmp(algorithm, "SCC"))) {
				total_vertices = num_vertices(g);
				vector<uint> component(total_vertices);
				uint total_components = strong_components(g, make_iterator_property_map(component.begin(), get(vertex_index, g)));
				uint i = 0;
				for (auto c: component) cout << i++ << " " << c << endl;
			} else if (!(strcmp(algorithm, "ts") && strcmp(algorithm, "TS"))) {
				if (is_dag(g)) {
					deque<uint> topological_order;
					topological_sort(g, front_inserter(topological_order));
					for (auto o: topological_order) cout << o << endl;
				} else cout << "not DAG" << endl;
			} else {
				cout << "Algorithm [" << algorithm << "] not available." << endl;
			}
		} else {
			print_graph(g);
		}
	}
}
