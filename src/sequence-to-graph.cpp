#include <iostream>
#include <list>
#include <string>

int main (int argc, char* argv[]) {
	using namespace std;

	typedef unsigned long long Value;

	unsigned short diffuse = 0;
	unsigned short alignment = 20;

	if (argc < 3) {
		cerr << "generator <diffuse> <alignment>" << endl;
		return -1;
	}

	try {
		size_t lastChar;
		diffuse = stoi(argv[1], &lastChar, 10);
		alignment = stoi(argv[2], &lastChar, 10);
	} catch(exception& e) {
		cerr << e.what() << endl;
		return -1;
	}

    string line;
	Value value = 0;
	list<Value> neighbours;
	while (getline(cin, line)) {
		try {
			int base = (line.compare(0, 2, "0x"))?16:10;
			value = stoull(line, nullptr, base);
		} catch(exception& e) {
			cerr << e.what() << endl;
			value = 0;
		}
		Value aligned = value >> alignment;
		for (auto n: neighbours) cout << n << "\t" << aligned << endl;
		neighbours.push_front(aligned);
		if (neighbours.size() > diffuse) neighbours.pop_back();
	}

	return 0;
}
