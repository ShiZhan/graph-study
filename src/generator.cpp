#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <time.h>

int main (int argc, char* argv[]) {
	using namespace std;

	// default values
	unsigned short v_factor = 10;
	unsigned short e_factor = 8;
	int seed[4] = {57, 19, 19, 5};

	if (argc < 3) {
		cerr << "generator <vertex(" << v_factor << ")> <edge(" << e_factor
			<< ")> [seed(" << seed[0] << ":" << seed[1] << ":" << seed[2] << ":" << seed[3] << ")]" << endl;
		return -1;
	}

	// get vertex scale and edge multiplier (average degree)
	try {
		size_t lastChar;
		v_factor = stoi(argv[1], &lastChar, 10);
		e_factor = stoi(argv[2], &lastChar, 10);
	} catch(exception& e) {
		cerr << e.what() << endl;
		return -1;
	}

	// get seed matrix (4 ranges)
	if (argc > 3) {
		istringstream ss(argv[3]);
		string s;
		vector<int> sv;
		try {
			size_t lastChar;
			while (getline(ss, s, ':')) sv.push_back(stoi(s, &lastChar, 10));
			if (sv.size() == 4) for (int i = 0; i < 4; i++) seed[i] = sv[i];
		} catch(exception& e) {
			cerr << e.what() << endl;
			return -1;
		}
	}

	srand((unsigned int)time(NULL));

	unsigned long long V = 1 << v_factor; // 2^v_factor vertices
	unsigned long long E = V * e_factor;  // 2^v_factor * e_factor edges
	int A = seed[0], B = A + seed[1], C = B + seed[2], D = C + seed[3];
	// generate edge list
	for (unsigned long long e = 0; e < E; e++) {
		unsigned long long u = 0, v = 0;
		for (unsigned short i = 0; i < v_factor; i++) {
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
