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

#define SOURCE 0

int main(int argc, char* argv[]) {
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "bgl-shell -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -i:\t (cin) input edge list" << endl;
		cout << " -e:\t perform [BFS|DFS|SSSP], etc." << endl;
		cout << " -s:\t (" << SOURCE << ") specify source vertex" << endl;
		return 0;
	}

	char* edges_file = getOption(argv, argv + argc, "-i");
	char* algorithm  = getOption(argv, argv + argc, "-e");
	int   source     = getInt(argv, argv + argc, "-s", SOURCE);

	Graph g;
	uint n_edges = get_edges(edges_file, g);

	if (algorithm) {
		if (!(strcmp(algorithm, "bfs") && strcmp(algorithm, "BFS"))) { cout << "Do BFS" << endl; }
		else if (!(strcmp(algorithm, "dfs") && strcmp(algorithm, "DFS"))) { cout << "Do DFS" << endl; }
		else if (!(strcmp(algorithm, "sssp") && strcmp(algorithm, "SSSP"))) { cout << "Do SSSP" << endl; }
		else { cout << "Available algorithms: BFS, DFS, SSSP." << endl; }
	} else print_adjacency_list(g);
}
