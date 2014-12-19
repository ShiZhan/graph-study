#include <iostream>
#include <list>
#include <string>
#include "options.h"

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	typedef unsigned long long Value;

	// default values
	unsigned short diffuse = 8;
	unsigned short alignment = 20;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "seq2graph -d [diffuse] -a [alignment]" << endl;
		cout << " -h:    \t ask for help" << endl;
		cout << " -d (" << diffuse   << "):\t diffuse factor" << endl;
		cout << " -a (" << alignment << "):\t alignment factor" << endl;
		return 0;
	}

	char* diffuse_str   = getOption(argv, argv + argc, "-d"); 
	char* alignment_str = getOption(argv, argv + argc, "-a"); 

	// get diffuse factor
	if (diffuse_str) {
		size_t lastChar;
		try { diffuse = stoi(diffuse_str, &lastChar, 10); }
		catch(exception& e) { cerr << e.what() << endl; }
	}

	// get alignment factor
	if (alignment_str) {
		size_t lastChar;
		try { alignment = stoi(alignment_str, &lastChar, 10); }
		catch(exception& e) { cerr << e.what() << endl; }
	}

	// read sequence, generate edges from history with given depth.
    string line;
	Value value = 0;
	list<Value> neighbours;
	while (getline(cin, line)) {
		try {
			int base = (line.compare(0, 2, "0x"))?16:10; // hex string check
			value = stoull(line, nullptr, base);
		} catch(exception& e) {
			cerr << e.what() << endl;
			value = 0;
		}
		// generate a vertex from current value within a certain range (2^alignment)
		Value aligned = value >> alignment;
		// edge: past n vertices to current vertex, assuming certain relationship
		for (auto n: neighbours) cout << n << "\t" << aligned << endl;
		// add current vertex to past n (<= diffuse) vertices
		neighbours.push_front(aligned);
		if (neighbours.size() > diffuse) neighbours.pop_back();
	}

	return 0;
}
