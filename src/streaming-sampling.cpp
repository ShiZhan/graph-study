#include <iostream>
#include <string>
#include <set>
#include <map>
#include "options.h"
#include "utils.h"

class BidirectionalGraph {
	typedef std::map<uint, std::set<uint>> graph_t;
private:
	graph_t g;
	graph_t::iterator i, j;
public:
	void add_edge(uint u, uint v) {
		if (u != v) {
			g[u].insert(v);
			g[v].insert(u);
		}
	}

	void remove_edge(uint u, uint v) {
		i = g.find(u); j = g.end(); if (i != j) i->second.erase(v);
		i = g.find(v); j = g.end(); if (i != j) i->second.erase(u);
	}

	void remove_vertex(uint v) {
		for (uint n: g[v]) g[n].erase(v);
		g[v].clear();// delete &g[v];
		g.erase(v);
	}

	uint sum_vertices() {
		return g.size();
	}

	uint sum_edges() {
		uint sum = 0;
		for (auto s: g) sum += s.second.size();
		return (sum / 2);
	}

	uint degree(uint v) {
		return g[v].size();
	}

	bool isolate(uint v) {
		return g[v].empty();
	}

	void print_edges() {
		for (auto s: g) {
			for (uint n: s.second) {
				std::cout << s.first << " " << n << std::endl;
			}
		}
	}

	void print_vertices() {
		for (auto s: g) {
			std::cout << s.first;
			for (uint n: s.second) {
				std::cout << " " << n;
			}
			std::cout << std::endl;
		}
	}
};

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	const char* usage =
		"streaming-sampling [options]\n"
		" -h:\t ask for help\n"
		" -s:\t subgraph vertices maximum size\n"
		" -r:\t subgraph/original graph vertices ratio\n"
		" -p:\t sampling policy\n"
		" -l:\t output log instead of subgraph\n"
		"\n";

	if (chkOption(argv, argv + argc, "-h")) {
		cout << usage;
		return 0;
	}

	uint   size_max = getInt(argv, argv + argc, "-s", 1024 * 8);
	double ratio    = getDouble(argv, argv + argc, "-r", 0);
	char*  policy   = getOption(argv, argv + argc, "-p");
	bool   show_log = chkOption(argv, argv + argc, "-l");

	BidirectionalGraph g;
	string line;
	uint u, v, size_origin, size_sample, size_candidate;
	set<uint> vertices_ori;

	while (getline(cin, line)) {
		SSCANF((line.c_str(), "%u %u", &u, &v));
		if (show_log) cout << "a " << u << " " << v << endl;
		g.add_edge(u, v);
		vertices_ori.insert(u);
		vertices_ori.insert(v);
		if (ratio > 0) {
			size_origin = vertices_ori.size();
			size_candidate = (uint)(size_origin * ratio);
			if (size_candidate > size_max) size_max = size_candidate;
		}
		size_sample = g.sum_vertices();
		if (size_sample > size_max) {
			// TODOs: remove vertices/edges from subgraph by using some policy
			if (show_log) cout << "d " << size_sample << "/" << size_max << endl;
		}
	}

	if (!show_log) g.print_edges();

	return 0;
}
