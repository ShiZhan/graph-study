#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "options.h"

#define CHUNK_SCALE 25

typedef unsigned long long u64;

class Edge {
public:
	u64 n[2];
	Edge(u64 u, u64 v) { n[0] = u; n[1] = v; }

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

class Chunk {
public:
	Chunk (u64 start, u64 size): start(start), size(size) { offset = start; }
	u64 current() { return offset; }
	bool next() { return (offset++==start+size); }
private:
	u64 start;
	u64 size;
	u64 offset;
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
		cout << " -c:\t (" << CHUNK_SCALE << ") sort [2^c] edges in memory" << endl;
		return 0;
	}

	u64 chunk_size = 1 << getInt(argv, argv + argc, "-c", CHUNK_SCALE);

	// sort input chunk by chunk
	string line;
	u64 u, v, chunk_total = 0;
	list<Edge> edges;
	fstream temp_file("_sorted_chunks", ios::trunc|ios::in|ios::out|ios::binary);
	while (getline(cin, line)) {
#ifdef _MSC_VER
		sscanf_s(line.c_str(), "%llu %llu", &u, &v);
#else
		sscanf(line.c_str(), "%llu %llu", &u, &v);
#endif
		edges.push_back(Edge(u, v));
		if (edges.size() >= chunk_size) {
			edges.sort();
			for (Edge e: edges) temp_file.write((char*)e.n, sizeof(u64)*2);
			chunk_total++;
			edges.clear();
		}
	}

	edges.sort();

	if (chunk_total) {
		// final chunk
		for (Edge e: edges) temp_file.write((char*)e.n, sizeof(u64)*2);
		// merge index (chunk_total way)
		list<Chunk> chunks;
		for (u64 i=0;i<chunk_total;i++) chunks.push_back(Chunk(i*chunk_size, chunk_size));
		chunks.push_back(Chunk(chunk_total*chunk_size, edges.size()));
		// merge sort
		Edge edge_previous(0, 0);
		for (Chunk c: chunks) cout << c.current() << endl;
		// clean up
		chunks.clear();
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
