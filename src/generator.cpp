#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <time.h>

int main (int argc, char* argv[]) {
	using namespace std;

	if (argc < 3) {
		cerr << "generator <vertex> <edge> [<seed(A:B:C:D)>]" << endl;
		return -1;
	}

	unsigned short v_factor = 10;
	unsigned short e_factor = 8;

	try {
		size_t lastChar;
		v_factor = stoi(argv[1], &lastChar, 10);
		e_factor = stoi(argv[2], &lastChar, 10);
	} catch(exception& e) {
		cerr << e.what() << endl;
		return -1;
	}

	int seed[4] = {57, 19, 19, 5};

	if (argc > 3) {
		istringstream ss(argv[3]);
		string s;
		vector<int> sv;
		try {
			size_t lastChar;
			while (getline(ss, s, ':')) sv.push_back(stoi(s, &lastChar, 10));
			if (sv.size() == 4) {
				int i = 0;
				for(auto v:sv) {
					seed[i] = v;
					i++;
				}
			}
		} catch(exception& e) {
			cerr << e.what() << endl;
			return -1;
		}
	}

	srand((unsigned int)time(NULL));

	unsigned long long V = 1 << v_factor;
	unsigned long long E = V * e_factor;
	int A = seed[0], B = A + seed[1], C = B + seed[2], D = C + seed[3];

	for (unsigned long long e = 0; e < E; e++) {
		unsigned long long u = 0, v = 0;
		for (unsigned short i = 0; i < v_factor; i++) {
			int roll = rand() % D;
			unsigned long long u1 = u << 1, v1 = v << 1;
			if (roll < A) {u = u1; v = v1;}
			else if (roll < B) {u = u1 + 1; v = v1;}
			else if (roll < C) {u = u1; v = v1 + 1;}
			else {u = u1 + 1; v = v1 + 1;}
		}
	    cout << u << " " << v << endl;
	}

	return 0;
}
