#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "csr.h"
#include "options.h"
#include "utils.h"

#define PREFIX "csr" // CSR indices name prefix default
#define ROOT 0       // source vertex default

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "dfs -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -p:\t (" << PREFIX << ") file name prefix for column and row index" << endl;
		cout << " -r:\t (" << ROOT   << ") root vertex" << endl;
		return 0;
	}

	char*      prefix_str = getOption(argv, argv + argc, "-p");
	uint64_t   root       = getUInt64(argv, argv + argc, "-r", ROOT);

	string prefix = prefix_str ? prefix_str : PREFIX; // get prefix

	CSR g;
	uint64_t vertex_total = g.load(prefix);
	if (vertex_total == 0) {
		cerr << "CSRGraph load error!" << endl;
		return -1;
	}

	bool* visited = new bool[vertex_total+1];
	uint64_t i;
	for (i=0; i<=vertex_total; i++) visited[i] = false;
	while (root<vertex_total) {
		stack<uint64_t> traversal;
		traversal.push(root);
		vector<uint64_t> neighbours;
		while(!traversal.empty()) {
			i = traversal.top();
			traversal.pop();
			if (!visited[i]) {
				cout << i << endl;
				visited[i] = true;
				uint64_t total = g.getNeighbours(i, neighbours);
				if (total) {
					for (uint64_t n: neighbours) traversal.push(n);
					neighbours.clear();
				}
			}
		}
		root = 0;
		while (visited[root]) root++;
	}

	delete[] visited;

	g.unload();

	return 0;
}
