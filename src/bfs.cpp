#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "csr.h"
#include "options.h"
#include "utils.h"

//#define PREFIX "csr" // CSR indices name prefix default
#define PREFIX "c:/msys/home/Zhan/repository/graph-study/data-sample/8-8-n-u" // CSR indices name prefix default
#define ROOT 3     // source vertex default

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "sssp -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -p:\t (" << PREFIX << ") file name prefix for column and row index" << endl;
		cout << " -r:\t (" << ROOT   << ") root vertex" << endl;
		return 0;
	}

	char* prefix_str = getOption(argv, argv + argc, "-p");
	u64   root       = getU64(argv, argv + argc, "-r", ROOT);

	string prefix = prefix_str ? prefix_str : PREFIX; // get prefix

	CSR g;
	u64 vertex_total = g.load(prefix);
	if (vertex_total == 0) {
		cerr << "CSRGraph load error!" << endl;
		return -1;
	}

	bool* visited = new bool[vertex_total];
	u64 i, d;
	for (i=0; i<vertex_total; i++) visited[i] = false;
	queue<u64> traversal;
	traversal.push(root);
	vector<u64> neighbours;
	while(!traversal.empty()) {
		i = traversal.front();
		traversal.pop();
		if (!visited[i]) {
			cout << i << endl;
			visited[i] = true;
			u64 total = g.getNeighbours(i, neighbours);
			if (total) {
				for (u64 n: neighbours) {
					if (!visited[n]) {
						traversal.push(n);
					}
				}
				neighbours.clear();
			}
		}
	}

	delete[] visited;

	g.unload();

	return 0;
}
