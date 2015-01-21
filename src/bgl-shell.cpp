#include <iostream>
#include <fstream>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include "options.h"
#include "utils.h"

int main(int argc, char* argv[]) {
	using namespace std;
	using namespace boost;
	using namespace opt;

	typedef pair<uint, uint> Edge;
	typedef adjacency_list<setS, vecS, directedS, no_property> Graph;
	typedef Graph::vertex_iterator    ItrVertex;
	typedef Graph::adjacency_iterator ItrNeighbour;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "generator -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -i:\t (cin) input edge list" << endl;
		return 0;
	}

	char* edges_file = getOption(argv, argv + argc, "-i");
	string line;
	uint u, v, N = 0;
	vector<Edge> edges;
	if (edges_file) {
		ifstream ifs(edges_file);
		while (getline(ifs, line)) {
			SSCANF((line.c_str(), "%u %u", &u, &v));
			edges.push_back(Edge(u, v));
			if (u > N) N = u;
			if (v > N) N = v;
		}
		ifs.close();
	} else {
		while (getline(cin, line)) {
			SSCANF((line.c_str(), "%u %u", &u, &v));
			edges.push_back(Edge(u, v));
			if (u > N) N = u;
			if (v > N) N = v;
		}
	}

	Graph g(edges.begin(), edges.end(), ++N);
	ItrVertex vertexIt, vertexEnd;
	ItrNeighbour neighbourIt, neighbourEnd;
	boost::tie(vertexIt, vertexEnd) = vertices(g);
	for (; vertexIt != vertexEnd; ++vertexIt) {
		cout << *vertexIt << " --> ";
		boost::tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, g);
		for (; neighbourIt != neighbourEnd; ++neighbourIt) cout << *neighbourIt << " ";
		cout << "\n";
	}
}
