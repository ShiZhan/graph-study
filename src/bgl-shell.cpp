#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <hash_map>
#include <map>
#include <deque>
#include <time.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/topological_sort.hpp>

#include <boost/graph/cuthill_mckee_ordering.hpp>
#include <boost/graph/king_ordering.hpp>
#include <boost/graph/minimum_degree_ordering.hpp>
#include <boost/graph/sloan_ordering.hpp>
#include <boost/graph/properties.hpp>

#include <boost/graph/rmat_graph_generator.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/graph/small_world_generator.hpp>
#include <boost/graph/plod_generator.hpp>
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

int graph_gen(char* generator, char* parameters) {
	uint total_vertices = 256, total_edges = 0;
	boost::minstd_rand gen;
	gen.seed((uint)time(NULL));

	if (!generator || !(strcmp(generator, "rmat") && strcmp(generator, "RMAT"))) {
		typedef rmat_iterator<boost::minstd_rand, adjacency_list<>> rmat_gen;
		uint scale_v = 8, scale_e = 8;
		if (parameters) SSCANF((parameters, "%u:%u", &scale_v, &scale_e));
		total_vertices = 1 << scale_v;
		total_edges    = total_vertices * scale_e;
		auto it       = rmat_gen(gen, total_vertices, total_edges, 0.57, 0.19, 0.19, 0.05);
		auto it_last  = rmat_gen();
		for (;it != it_last; ++it) cout << it->first << " " << it->second << endl;
	} else if (!(strcmp(generator, "er") && strcmp(generator, "ER"))) {
		typedef erdos_renyi_iterator<boost::minstd_rand, adjacency_list<>> er_gen;
		double probability = 0.05;
		if (parameters) SSCANF((parameters, "%u:%lf", &total_vertices, &probability));
		auto it       = er_gen(gen, total_vertices, probability);
		auto it_last  = er_gen();
		for (;it != it_last; ++it) cout << it->first << " " << it->second << endl;
	} else if (!(strcmp(generator, "sw") && strcmp(generator, "SW"))) {
		typedef small_world_iterator<boost::minstd_rand, adjacency_list<>> sw_gen;
		uint   knn         = 6;
		double probability = 0.03;
		if (parameters) SSCANF((parameters, "%u:%u:%lf", &total_vertices, &knn, &probability));
		auto it       = sw_gen(gen, total_vertices, knn, probability);
		auto it_last  = sw_gen();
		for (;it != it_last; ++it) cout << it->first << " " << it->second << endl;
	} else if (!(strcmp(generator, "sf") && strcmp(generator, "SF"))) {
		typedef plod_iterator<boost::minstd_rand, adjacency_list<>> sf_gen;
		double alpha = 2.7;
		uint   beta  = 256;
		if (parameters) SSCANF((parameters, "%u:%lf:%u", &total_vertices, &alpha, &beta));
		auto it       = sf_gen(gen, total_vertices, alpha, beta);
		auto it_last  = sf_gen();
		for (;it != it_last; ++it) cout << it->first << " " << it->second << endl;
	} else {
		cout << "Available generators: RMAT, ER, SW, SF." << endl;
		return -1;
	}
	return 0;
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

template <class Graph>
bool isSymmetric(const Graph &g) {
    std::pair<graph_traits<adjacency_list<setS, vecS, directedS> >::edge_descriptor, bool> tmp;
	BGL_FORALL_EDGES_T(e, g, Graph){
        tmp = edge(target(e, g), source(e, g), g);
        if(tmp.second==false)return false;
    }
	return true;
}

template <class Graph>
int dir_graph_op(Graph& g, char* algorithm, uint v_root) {
	if (!(strcmp(algorithm, "bfs") && strcmp(algorithm, "BFS"))) {
		custom_bfs_visitor b_v;
		breadth_first_search(g, vertex(v_root, g), visitor(b_v));
	} else if (!(strcmp(algorithm, "dfs") && strcmp(algorithm, "DFS"))) {
		custom_dfs_visitor d_v;
		depth_first_search(g, root_vertex(vertex(v_root, g)).visitor(d_v));
	} else if (!(strcmp(algorithm, "scc") && strcmp(algorithm, "SCC"))) {
		uint total_vertices = num_vertices(g);
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
	} else if(!(strcmp(algorithm, "mdo") && strcmp(algorithm, "MDO"))) {
		if(isSymmetric(g)) {
			uint n = num_vertices(g);
        	vector<int> degree(n, 0);
        	vector<int> supernode_sizes(n, 1);
        	vector<int> inv_perm(n, 0);//build_permutation(InversePermutationMap next,PermutationMap prev)(minimum_degree_ordering.hpp)
        	vector<int> perm(n, 0); //prev[i] and next[i] can be negative,so vector inv_perm and perm shouldn't be uint 
        	minimum_degree_ordering(g,
                                make_iterator_property_map(&degree[0], get(vertex_index,g), degree[0]),
                                &inv_perm[0],
                                &perm[0],
                                make_iterator_property_map(&supernode_sizes[0],get(vertex_index,g) ,supernode_sizes[0]),
                                0, get(vertex_index,g));
        	for(auto p: inv_perm) cout << p << endl;	
		} else cout << "The metrix of graph is not SYMMETRIC !" << endl;
	} else {
		cout << "Algorithm [" << algorithm << "] is not available (for directed graph)." << endl;
		return -1;
	}
	return 0;
}

template <class Graph>
int undir_graph_op(Graph& g, char* algorithm, uint v_root) {
	if (!(strcmp(algorithm, "bfs") && strcmp(algorithm, "BFS"))) {
		custom_bfs_visitor b_v;
		breadth_first_search(g, vertex(v_root, g), visitor(b_v));
	} else if (!(strcmp(algorithm, "dfs") && strcmp(algorithm, "DFS"))) {
		custom_dfs_visitor d_v;
		depth_first_search(g, root_vertex(vertex(v_root, g)).visitor(d_v));
	} else if (!(strcmp(algorithm, "cmo") && strcmp(algorithm, "CMO"))) {
		vector<uint> cuthill_mckee_order(num_vertices(g));
		cuthill_mckee_ordering(g, cuthill_mckee_order.rbegin()); 
		for(auto cmo: cuthill_mckee_order) cout << cmo << endl;
	} else if (!(strcmp(algorithm, "ko") && strcmp(algorithm, "KO"))) {
		vector<uint> king_order(num_vertices(g));
		king_ordering(g, king_order.rbegin());
		for(auto ko: king_order) cout << ko << endl;
	} else if(!(strcmp(algorithm, "so") && strcmp(algorithm, "SO"))) {
		//the graph need state some properties
		vector<uint> sloan_order(num_vertices(g));
		sloan_ordering(g, sloan_order.begin(), get(vertex_color, g), make_degree_map(g), get(vertex_priority, g));
		for(auto so: sloan_order) cout<< so <<endl;
	} else {
		cout << "Algorithm [" << algorithm << "] is not available (for undirected graph)." << endl;
		return -1;
	}
	return 0;
}

template <class Graph>
void print_edges(const Graph& g) {
	BGL_FORALL_EDGES_T(e, g, Graph)
		cout << source(e, g) << " " << target(e, g) << endl;
}

#define DEFAULT_ROOT 0

int main(int argc, char* argv[]) {
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "bgl-shell [options]" << endl
			<< " -h:\t ask for help" << endl << endl
			<< " graph type" << endl
			<< " default to set the graph directed" << endl
			<< " -u:\t state that the graph is undirected" << endl << endl
			<< " generators" << endl
			<< " -g:\t (RMAT) use generator [RMAT|ER|SW|SF]" << endl
			<< " -p:\t set graph generator parameters" << endl
			<< " \t   Generator         Parameters"  << endl
			<< " \t   ---------         ----------"  << endl
			<< " \t   Recursive-MATrix  8:8"         << endl
			<< " \t   Erdos-Renyi       256:0.05"    << endl
			<< " \t   Small-World       256:6:0.03"  << endl
			<< " \t   Scale-Free        256:2.7:256" << endl << endl
			<< " algorithms" << endl
			<< " default to print adjacency list" << endl
			<< " -e:\t perform [BFS|DFS|SCC|TS|CMO|MDO], etc." << endl
			<< " \t   BFS: breadth-first traversal (on directed or undirected graph)" << endl
			<< " \t   DFS: depth-first traversal (on directed graph)" << endl
			<< " \t   SCC: strongly connected components (on directed graph)" << endl
			<< " \t   TS:  topological sort (on directed acyclic graph only)" << endl
			<< " \t   MDO: minimum degree ordering (on directed graph)" << endl
			<< " \t   CMO: cuthill mckee ordering (on undirected graph)" << endl
			<< " \t   KO:  king ordering (on undirected graph)" << endl
			<< " \t   SO:  sloan ordering (on undirected graph)" << endl
			<< " -i:\t (cin) input edge list" << endl
			<< " -r:\t (" << DEFAULT_ROOT << ") specify root vertex for graph traversal" << endl;
		return 0;
	}

	bool  use_gen    = chkOption(argv, argv + argc, "-g");
	bool  is_undir   = chkOption(argv, argv + argc, "-u");
	char* generator  = getOption(argv, argv + argc, "-g");
	char* gen_param  = getOption(argv, argv + argc, "-p");
	char* edges_file = getOption(argv, argv + argc, "-i");
	char* algorithm  = getOption(argv, argv + argc, "-e");
	uint  v_root     = getInt(argv, argv + argc, "-s", DEFAULT_ROOT);

	typedef adjacency_list<setS, vecS,   directedS, no_property> graph_t;
	typedef adjacency_list<setS, vecS, undirectedS, no_property> graph_u_t;
	typedef adjacency_list<
		setS, 
		vecS, 
		undirectedS, 
		property<
		vertex_color_t, 
		default_color_type, 
		property<
		vertex_degree_t,
		int,
		property<
		vertex_priority_t,
		double > > > > graph_u_p_t;

	if(use_gen) {
		//TODO  here only generate vertex pairs, maybe need to load into a graph
		graph_gen(generator, gen_param); 
	}

	if (is_undir) {
		graph_u_p_t g;
		//graph_u_t g;
		get_edges(edges_file, g);
		if(algorithm) {
			undir_graph_op(g, algorithm, v_root);
		}
	} else {
		graph_t g;
		get_edges(edges_file, g);
		if(algorithm) {
			dir_graph_op(g, algorithm, v_root);
		}
	}
}
