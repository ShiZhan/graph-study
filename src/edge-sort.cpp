#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include "options.h"
#include "utils.h"

#define CHUNK_SCALE 20 // 2^20*16 Bytes = 16 M Bytes
#define _TEMP_FILE_ "_sorted_chunks"

class Edge {
public:
	u64 n[2];
	Edge () { n[0] = -1; n[1] = -1; }
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
		start(start), size(size), f(f) { offset = start; end = start + size; }

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
		cout << " -c:\t (" << CHUNK_SCALE << ") sort [" << (1 << CHUNK_SCALE) << "] edges per chunk" << endl;
		return 0;
	}

	u64 chunk_limit = 1 << getInt(argv, argv + argc, "-c", CHUNK_SCALE);
	bool use_tempfile = chkOption(argv, argv + argc, "-c");

	string line;
	u64 u, v;
	vector<Edge> edges;

	if (use_tempfile) {
		fstream temp_file(_TEMP_FILE_, ios::trunc|ios::in|ios::out|ios::binary);
		u64 chunk_offset = 0, chunk_size = 0;
		list<Chunk> chunks; // all [chunk_total] chunks merge indices

		bool more = true;
		while(more) {
			if (getline(cin, line)) {
				SSCANF((line.c_str(), "%llu %llu", &u, &v));
				edges.push_back(Edge(u, v));
				more = true;
			} else
				more = false;

			chunk_size = edges.size();
			if ((!more && chunk_size > 0) || (chunk_size >= chunk_limit)) {
				sort(edges.begin(), edges.end());
				for (Edge e: edges) temp_file.write((char*)e.n, sizeof(u64)*2);
				edges.clear();
				chunks.push_back(Chunk(chunk_offset, chunk_size, temp_file));
				chunk_offset += chunk_size;
			}
		}

		for (auto &c: chunks) c.next(); // select first item from each chunk
		Edge edge_previous;
		list<Chunk>::iterator min;
		while (chunks.size()>0) { // merge sort
			min = min_element(chunks.begin(), chunks.end());
			if (*min != edge_previous) {
				cout << *min;
				edge_previous = *min;
			}
			if (min->next()==0) chunks.erase(min);
		}

		temp_file.close();
	} else {
		while (getline(cin, line)) {
			SSCANF((line.c_str(), "%llu %llu", &u, &v));
			edges.push_back(Edge(u, v));
		}
		sort(edges.begin(), edges.end());
		Edge edge_previous;
		for (Edge e: edges) {
			if (e != edge_previous) {
				cout << e;
				edge_previous = e;
			}
		}
		edges.clear();
	}

	return 0;
}
