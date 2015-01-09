#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "graph.h"
#include "options.h"
#include "utils.h"

#define PREFIX "c:/msys/home/Zhan/repository/graph-study/data-sample/15-112-n-u" // CSR indices name prefix default
#define SOURCE 3     // source vertex default

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "sssp -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -p:\t (" << PREFIX << ") file name prefix for column and row index" << endl;
		cout << " -s:\t (" << SOURCE << ") source vertex" << endl;
		return 0;
	}

	char* prefix_str = getOption(argv, argv + argc, "-p");
	u64   source     = getU64(argv, argv + argc, "-s", SOURCE);

	string prefix = prefix_str ? prefix_str : PREFIX; // get prefix

	CSRGraph g;
	u64 vertex_total = g.load(prefix);
	if (vertex_total == 0) {
		cerr << "CSRGraph load error!" << endl;
		return -1;
	}

	vector<u64> neighbours;
	u64 total = g.getNeighbours(source, neighbours);
	for (u64 n: neighbours) cout << n << endl;
	neighbours.clear();

	g.unload();

	return 0;
}
