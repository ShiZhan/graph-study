#include <iostream>
#include <fstream>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include "options.h"
#include "utils.h"

using namespace std;
using namespace boost;
typedef adjacency_list<setS, vecS, directedS, no_property> Graph;

uint get_edges(char* edges_file, Graph &g) {
	uint u, v, total = 0;
	string line;
	if (edges_file) {
		ifstream ifs(edges_file);
		while (getline(ifs, line)) {
			SSCANF((line.c_str(), "%u %u", &u, &v));
			add_edge(u, v, g);
			++total;
		}
		ifs.close();
	} else {
		while (getline(cin, line)) {
			SSCANF((line.c_str(), "%u %u", &u, &v));
			add_edge(u, v, g);
			++total;
		}
	}
	return total;
}

void print_adjacency_list(Graph &g) {
	Graph::adjacency_iterator neighbourIt, neighbourEnd;
	for (auto v: g.vertex_set()) {
		cout << v << " --> ";
		boost::tie(neighbourIt, neighbourEnd) = adjacent_vertices(v, g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt) cout << *neighbourIt << " ";
		cout << endl;
	}
}

int main(int argc, char* argv[]) {
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "bgl-shell -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -i:\t (cin) input edge list" << endl;
		cout << " -e:\t perform [BFS|DFS|SSSP], etc." << endl;
		return 0;
	}

	char* edges_file = getOption(argv, argv + argc, "-i");
	char* algorithm  = getOption(argv, argv + argc, "-e");

	Graph g;
	uint n_edges = get_edges(edges_file, g);

	if (algorithm) {
		cout << "TODO" << endl;
	} else print_adjacency_list(g);
}
