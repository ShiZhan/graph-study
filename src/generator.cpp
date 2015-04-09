#include <iostream>
#include <time.h>
#include "options.h"
#include "utils.h"

#define SCALE 8
#define D_AVG 16

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	int seed[4] = {57, 19, 19, 5}; // default seed matrix

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

	uint16_t scale      = getInt(argv, argv + argc, "-s", SCALE);
	uint16_t degree     = getInt(argv, argv + argc, "-d", D_AVG);
	char*    seed_str   = getOption(argv, argv + argc, "-m");
	bool     noloop     = chkOption(argv, argv + argc, "-n");
	bool     undirected = chkOption(argv, argv + argc, "-u");

	// get seed matrix (4 ranges)
	if (seed_str) SSCANF((seed_str, "%u:%u:%u:%u", seed, seed+1, seed+2, seed+3));

	srand((uint32_t)time(NULL));

	uint64_t E = (1 << scale) * degree / 2;  // 2^scale vertices * degree / 2 edges
	int A = seed[0],     B = A + seed[1]; // (0, 0) A | (0, 1) B
	int C = B + seed[2], D = C + seed[3]; // (1, 0) C | (1, 1) D
	for (uint64_t e = 0; e < E; e++) { // generate edge list
		uint64_t u = 0, v = 0;
		for (uint16_t i = 0; i < scale; i++) {
			int roll = rand() % D;       // [0, D) roll dice
			u <<= 1; v <<= 1;            // [0, A)
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
