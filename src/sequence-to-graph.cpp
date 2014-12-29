#include <iostream>
#include <list>
#include <string>
#include "options.h"

#define DIFFUSE   8
#define ALIGNMENT 20

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	typedef unsigned long long ID;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "seq2graph -d [diffuse] -a [alignment]" << endl;
		cout << " -h:    \t ask for help" << endl;
		cout << " -d (" << DIFFUSE   << "):\t diffuse factor" << endl;
		cout << " -a (" << ALIGNMENT << "):\t alignment factor" << endl;
		return 0;
	}

	unsigned short diffuse   = getInt(argv, argv + argc, "-d", DIFFUSE);
	unsigned short alignment = getInt(argv, argv + argc, "-a", ALIGNMENT);

	// read sequence, generate edges from history with given depth.
    string line;
	ID value = 0;
	list<ID> neighbours;
	while (getline(cin, line)) {
		try {
			int base = (line.compare(0, 2, "0x"))?16:10; // hex string check
			value = stoull(line, nullptr, base);
		} catch(exception& e) {
			cerr << e.what() << endl;
			value = 0;
		}
		// generate a vertex from current value within a certain range (2^alignment)
		ID aligned = value >> alignment;
		// edge: past n vertices to current vertex, assuming certain relationship
		for (auto n: neighbours) cout << n << " " << aligned << endl;
		// add current vertex to past n (<= diffuse) vertices
		neighbours.push_front(aligned);
		if (neighbours.size() > diffuse) neighbours.pop_back();
	}

	return 0;
}
