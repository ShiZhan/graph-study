#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "csr.h"
#include "options.h"
#include "utils.h"

#define PREFIX "csr" // CSR indices name prefix default
//#define PREFIX "c:/msys/home/Zhan/repository/graph-study/data-sample/8-8-n-u"
#define SOURCE 3     // source vertex default

class Vertex
{
public:
	uint64_t value;
	bool visited;
	Vertex() { value = -1; visited = false; }
	~Vertex() {}
	bool operator < (const Vertex &v) const {
		return value < v.value;
	}
};

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

	char*    prefix_str = getOption(argv, argv + argc, "-p");
	uint64_t source     = getUInt64(argv, argv + argc, "-s", SOURCE);

	string prefix = prefix_str ? prefix_str : PREFIX; // get prefix

	CSR g;
	uint64_t vertex_total = g.load(prefix);
	if (vertex_total == 0) {
		cerr << "CSRGraph load error!" << endl;
		return -1;
	}

	uint64_t i, dist;
	Vertex* vertices = new Vertex[vertex_total];
	vertices[source].value = 0;
	list<uint64_t> traversal(1, source);
	list<uint64_t>::iterator current;
	vector<uint64_t> neighbours;
	while(!traversal.empty()) {
		current = min_element(traversal.begin(), traversal.end(),
			[=](uint64_t i, uint64_t j) { return vertices[i] < vertices[j]; });
		i = *current;
		traversal.erase(current);
		if (!vertices[i].visited) {
			dist = vertices[i].value;
			vertices[i].visited = true;
			uint64_t total = g.getNeighbours(i, neighbours);
			if (total) {
				for (uint64_t n: neighbours) {
					uint64_t newDist = dist + 1; // step over weight-1 edge
					if (newDist < vertices[n].value) vertices[n].value = newDist;
					traversal.push_back(n);
				}
				neighbours.clear();
			}
		}
	}
	for (i=0; i<vertex_total; i++) cout << i << "\t" << vertices[i].value << endl;

	delete[] vertices;

	g.unload();

	return 0;
}
