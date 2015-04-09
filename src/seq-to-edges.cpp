#include <iostream>
#include <list>
#include <string>
#include "options.h"
#include "utils.h"

#define DIFFUSE   8
#define ALIGNMENT 20

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "seq2graph -d [diffuse] -a [alignment]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -d:\t (" << DIFFUSE   << ") diffuse factor" << endl;
		cout << " -a:\t (" << ALIGNMENT << ") alignment factor" << endl;
		return 0;
	}

	uint16_t diffuse   = getInt(argv, argv + argc, "-d", DIFFUSE);
	uint16_t alignment = getInt(argv, argv + argc, "-a", ALIGNMENT);

    string line;
	uint64_t value = 0;
	list<uint64_t> neighbours;
	while (getline(cin, line)) { // read sequence
		try {
			int base = (line.compare(0, 2, "0x"))?16:10; // hex string check
			value = stoull(line, nullptr, base);
		} catch(exception& e) {
			cerr << e.what() << endl;
			value = 0;
		}
		// generate a vertex from current value within a certain range (2^alignment)
		uint64_t aligned = value >> alignment;
		// edge: past n vertices to current vertex, assuming certain relationship
		for (uint64_t n: neighbours) cout << n << " " << aligned << endl;
		// add current vertex to past n (<= diffuse) vertices
		neighbours.push_front(aligned);
		if (neighbours.size() > diffuse) neighbours.pop_back();
	}

	return 0;
}
