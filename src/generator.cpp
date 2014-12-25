#include <iostream>
#include <string>
#include <time.h>
#include "options.h"

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	// default values
	unsigned short scale  = 10;
	unsigned short degree = 16;
	int seed[4] = {57, 19, 19, 5};

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "generator -s [scale] -d [degree] -m [seed matrix]" << endl;
		cout << " -h:     \t ask for help" << endl;
		cout << " -s (" << scale  << "):\t 2^scale vertices" << endl;
		cout << " -d (" << degree << "):\t average degree" << endl;
		cout << " -m (" << seed[0] << ":" << seed[1] << ":" << seed[2] << ":" << seed[3] << "):\t seed matrix" << endl;
		return 0;
	}

	char* scale_str  = getOption(argv, argv + argc, "-s"); 
	char* degree_str = getOption(argv, argv + argc, "-d"); 
	char* seed_str   = getOption(argv, argv + argc, "-m"); 

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
		try { sscanf_s(seed_str, "%u:%u:%u:%u", seed, seed+1, seed+2, seed+3); }
		catch(exception& e) { cerr << e.what() << endl; }
	}

	srand((unsigned int)time(NULL));

	unsigned long long V = 1 << scale;      // 2^scale vertices
	unsigned long long E = V * degree / 2;  // 2^scale * degree / 2 edges
	int A = seed[0], B = A + seed[1], C = B + seed[2], D = C + seed[3];
	// generate edge list
	for (unsigned long long e = 0; e < E; e++) {
		unsigned long long u = 0, v = 0;
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
	    cout << u << " " << v << endl;
	}

	return 0;
}
