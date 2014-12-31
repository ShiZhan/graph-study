#include <iostream>
#include <time.h>
#include "options.h"
#include "types.h"

#define SCALE 8
#define D_AVG 16

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	// default seed matrix
	int seed[4] = {57, 19, 19, 5};

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "generator -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -s:\t (" << SCALE << ") 2^scale vertices" << endl;
		cout << " -d:\t (" << D_AVG << ") average degree" << endl;
		cout << " -m:\t (" << seed[0] << ":" << seed[1] << ":" << seed[2] << ":" << seed[3] << ") seed matrix" << endl;
		cout << " -n:\t no loop edges" << endl;
		cout << " -u:\t undirected" << endl;
		return 0;
	}

	u16   scale      = getInt(argv, argv + argc, "-s", SCALE);
	u16   degree     = getInt(argv, argv + argc, "-d", D_AVG);
	char* seed_str   = getOption(argv, argv + argc, "-m");
	bool  noloop     = chkOption(argv, argv + argc, "-n");
	bool  undirected = chkOption(argv, argv + argc, "-u");

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

	srand((u32)time(NULL));

	u64 E = (1 << scale) * degree / 2;  // 2^scale vertices * degree / 2 edges
	int A = seed[0], B = A + seed[1], C = B + seed[2], D = C + seed[3];
	// generate edge list
	for (u64 e = 0; e < E; e++) {
		u64 u = 0, v = 0;
		for (u16 i = 0; i < scale; i++) {
			int roll = rand() % D; // roll dice
			u <<= 1; v <<= 1;
			// (0, 0) A | (0, 1) B
			// -------------------
			// (1, 0) C | (1, 1) D
			if (roll >= C) { u++; v++; } // [C, D)
			else if (roll >= B) u++;     // [B, C)
			else if (roll >= A) v++;     // [A, B)
		}
		if (!noloop || (u!=v)) {
			cout << u << " " << v << endl;
			if (undirected) cout << v << " " << u << endl;
		}
	}

	return 0;
}
