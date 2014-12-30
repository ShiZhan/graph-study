#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "options.h"

#define CHUNK 25

typedef unsigned long long ID;

class Edge {
public:
	ID n[2];
	Edge(ID u, ID v) { n[0] = u; n[1] = v; }

	bool operator < (const Edge &e) const {
		return (n[0] == e.n[0]) ? (n[1] < e.n[1]) : (n[0] < e.n[0]);
	}

	bool operator == (const Edge &e) const {
		return (n[0] == e.n[0]) && (n[1] == e.n[1]);
	}

	bool operator != (const Edge &e) const {
		return (n[0] != e.n[0]) || (n[1] != e.n[1]);
	}
};

std::ostream &operator<<(std::ostream &o, const Edge e) {
	o << e.n[0] << " " << e.n[1] << std::endl;
	return o;
}

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "edge-sort -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -c:\t (" << CHUNK << ") sort [2^c] edges in memory" << endl;
		return 0;
	}

	unsigned long chunk = 1 << getInt(argv, argv + argc, "-c", CHUNK);

	// sort input chunk by chunk
	string line;
	ID u, v, chunk_total = 0;
	list<Edge> edges;
	fstream temp_file("_sorted_chunks", ios::trunc|ios::in|ios::out|ios::binary);
	while (getline(cin, line)) {
#ifdef _MSC_VER
		sscanf_s(line.c_str(), "%llu %llu", &u, &v);
#else
		sscanf(line.c_str(), "%llu %llu", &u, &v);
#endif
		edges.push_back(Edge(u, v));
		if (edges.size() >= chunk) {
			edges.sort();
			for (Edge e: edges) temp_file.write((char*)e.n, sizeof(ID)*2);
			chunk_total++;
			edges.clear();
		}
	}

	edges.sort();

	if (chunk_total) {
		// final chunk
		for (Edge e: edges) temp_file.write((char*)e.n, sizeof(ID)*2);
		// merge index (chunk_total way)
		list<ID> chunk_index;
		for (;chunk_total>0;chunk_total--) chunk_index.push_back(chunk);
		chunk_index.push_back(edges.size());
		// merge sort
		for (ID i: chunk_index) cout << i << endl;
		// clean up
		chunk_index.clear();
	} else {
		// smaller than 1 chunk, output to cout directly.
		Edge edge_previous(0, 0);
		for (Edge e: edges) {
			if (e != edge_previous) {
				cout << e;
				edge_previous = e;
			}
		}
	}

	edges.clear();
	temp_file.close();

	return 0;
}
