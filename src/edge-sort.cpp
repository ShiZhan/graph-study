#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "options.h"
#include "types.h"

#define CHUNK_SCALE 25 // 2^25*16 Bytes = 512 M Bytes
#define _TEMP_FILE_ "_sorted_chunks"

class Edge {
public:
	u64 n[2];
	Edge() { n[0] = -1; n[1] = -1; }
	Edge (u64 u, u64 v) { n[0] = u; n[1] = v; }

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

class Chunk: public Edge {
public:
	Chunk (u64 start, u64 size, std::fstream &f):
		start(start), size(size), f(f) {
			offset = start; end = start + size;
			next();
	}

	u64 next() {
		if (offset == end) return 0;
		else {
			u64 address = offset << 4; // multiply by sizeof(u64)*2
			f.seekg(address);
			f.read((char*)n, sizeof(u64)*2);
			return ++offset;
		}
	}
private:
	u64 start, end, size, offset;
	std::fstream &f;
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
	fstream temp_file(_TEMP_FILE_, ios::trunc|ios::in|ios::out|ios::binary);
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
		// prepare all [chunk_total] chunks merge indices
		list<Chunk> chunks;
		for (u64 i=0;i<chunk_total;i++)
			chunks.push_back(Chunk(i*chunk_size, chunk_size, temp_file));
		chunks.push_back(Chunk(chunk_total*chunk_size, edges.size(), temp_file));
		// merge sort
		Edge edge_previous;
		while (chunks.size()>0) {
			list<Chunk>::iterator min = min_element(chunks.begin(), chunks.end());
			if (*min != edge_previous) {
				cout << *min;
				edge_previous = *min;
			}
			if ((*min).next()==0) chunks.erase(min);
		}
	} else {
		// smaller than 1 chunk, output to cout directly.
		Edge edge_previous;
		for (Edge e: edges) {
			if (e != edge_previous) {
				cout << e;
				edge_previous = e;
			}
		}
	}

	// clean up
	edges.clear();
	temp_file.close();

	return 0;
}
