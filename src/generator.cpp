#include <iostream>
#include <time.h>
#include "options.h"

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	typedef unsigned long long ID;

	// default values
	unsigned short scale  = 10;
	unsigned short degree = 16;
	int seed[4] = {57, 19, 19, 5};

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "generator -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -s:\t (" << scale  << ") 2^scale vertices" << endl;
		cout << " -d:\t (" << degree << ") average degree" << endl;
		cout << " -m:\t (" << seed[0] << ":" << seed[1] << ":" << seed[2] << ":" << seed[3] << ") seed matrix" << endl;
		cout << " -n:\t no loop edges" << endl;
		cout << " -u:\t undirected" << endl;
		return 0;
	}

	char* scale_str  = getOption(argv, argv + argc, "-s"); 
	char* degree_str = getOption(argv, argv + argc, "-d"); 
	char* seed_str   = getOption(argv, argv + argc, "-m"); 
	bool  noloop     = chkOption(argv, argv + argc, "-n"); 
	bool  undirected = chkOption(argv, argv + argc, "-u"); 

	// get vertex scale
	if (scale_str) {
		size_t lastChar;
		try { scale = stoi(scale_str, &lastChar, 10); }
		catch(exception& e) { cerr << e.what() << endl; }
	}

	// get average degree
	if (degree_str) {
		size_t lastChar;
		try { degree = stoi(degree_str, &lastChar, 10); }
		catch(exception& e) { cerr << e.what() << endl; }
	}

	// get seed matrix (4 ranges)
	if (seed_str) {
		try {
#ifdef _MSC_VER
			sscanf_s(seed_str, "%u:%u:%u:%u", seed, seed+1, seed+2, seed+3);
#else
			sscanf(seed_str, "%u:%u:%u:%u", seed, seed+1, seed+2, seed+3);
#endif
		} catch(exception& e) { cerr << e.what() << endl; }
	}

	srand((unsigned int)time(NULL));

	ID E = (1 << scale) * degree / 2;  // 2^scale vertices * degree / 2 edges
	int A = seed[0], B = A + seed[1], C = B + seed[2], D = C + seed[3];
	// generate edge list
	for (ID e = 0; e < E; e++) {
		ID u = 0, v = 0;
		for (unsigned short i = 0; i < scale; i++) {
			int roll = rand() % D; // roll dice
			u <<= 1; v <<= 1;
			// (0, 0) A | (0, 1) B
			// -------------------
			// (1, 0) C | (1, 1) D
			if (roll >= C) { u ++; v ++; } // [C, D)
			else if (roll >= B) u ++;      // [B, C)
			else if (roll >= A) v ++;      // [A, B)
		}
		if (!noloop || (u!=v)) {
			cout << u << " " << v << endl;
			if (undirected) cout << v << " " << u << endl;
		}
	}

	return 0;
}
