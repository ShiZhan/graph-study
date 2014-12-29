#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "options.h"

#define TMP_D "."
#define CHUNK 25

class Edge {
public:
	Edge(unsigned long long u, unsigned long long v): u(u), v(v) {}
	unsigned long long u, v;
    bool operator < (const Edge &e) const {
		return (u == e.u) ? (v < e.v) : (u < e.u);
    }
    bool operator == (const Edge &e) const {
		return (u == e.u) && (v == e.v);
    }
};

std::ostream &operator<<(std::ostream &o, const std::list<Edge> edges) {
	for (Edge e:edges) { o << e.u << " " << e.v << std::endl; }
	return o;
}

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	typedef unsigned long long ID;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "edge-sort -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -t:\t (" << TMP_D << ") directory for temporary files" << endl;
		cout << " -c:\t (" << CHUNK << ") sort [2^c] edges in memory" << endl;
		return 0;
	}

	char* temp_dir_opt = getOption(argv, argv + argc, "-t");
	string temp_dir = (temp_dir_opt)?temp_dir_opt:TMP_D;
	unsigned long chunk = 1 << getInt(argv, argv + argc, "-c", CHUNK);

	// check temp directory
	struct stat st;
	if(stat(temp_dir.c_str(), &st) == 0) {
		if((st.st_mode & S_IFDIR) == 0) {
			cerr << temp_dir << " not exist" << endl;
			return -1;
		}
	} else {
		cerr << "stat() error" << endl;
		return -1;
	}

	// sort input line by line, then chunk by chunk
	string line;
	ID u, v, chunk_total = 0;
	list<Edge> edges;
	char file_name[255];
	string file_name_fmt = temp_dir + "/%08x";
	while (getline(cin, line)) {
#ifdef _MSC_VER
		sscanf_s(line.c_str(), "%llu %llu", &u, &v);
#else
		sscanf(line.c_str(), "%llu %llu", &u, &v);
#endif
		edges.push_back(Edge(u, v));
		if (edges.size() >= chunk) {
			edges.sort();
			edges.unique();
#ifdef _MSC_VER
			sprintf_s(file_name, file_name_fmt.c_str(), chunk_total);
#else
			sprintf(file_name, file_name_fmt.c_str(), chunk_total);
#endif
			ofstream chunk_file(file_name);
			chunk_file << edges;
			chunk_file.close();
			chunk_total++;
			edges.clear();
		}
	}

	edges.sort();
	edges.unique();
	if (chunk_total) {
		// final chunk
#ifdef _MSC_VER
		sprintf_s(file_name, file_name_fmt.c_str(), chunk_total);
#else
		sprintf(file_name, file_name_fmt.c_str(), chunk_total);
#endif
		ofstream chunk_file(file_name);
		chunk_file << edges;
		chunk_file.close();
		// merge sort
	} else {
		cout << edges; // smaller than 1 chunk
	}
	edges.clear();

	return 0;
}
