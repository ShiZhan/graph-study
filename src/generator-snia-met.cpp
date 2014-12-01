#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>

#define MAX_TOKENS 255

int main (int argc, char* argv[]) {
	using namespace std;

	unsigned short diffuse_factor = 0;
	unsigned short block_scale = 16;
	string filename;

	if (argc < 4) {
		cerr << "generator <trace.csv> <diffuse factor> <block scale>" << endl;
		return -1;
	}

	try {
		filename = argv[1];
		size_t lastChar;
		diffuse_factor = stoi(argv[2], &lastChar, 10);
		block_scale = stoi(argv[3], &lastChar, 10);
	} catch(exception& e) {
		cout << e.what() << endl;
		return -1;
	}

	ifstream infile(filename);
	if (!infile) return -1;

    string line;
	string *tokens = new string[MAX_TOKENS];
	list<unsigned long long> neighbours;
	bool skip = true;
	while (getline(infile, line)) {
		stringstream splitter(line);
		string token;
		int i = 0;
		while(getline(splitter, token, ',')) {
			tokens[i] = token.erase(0, token.find_last_of(" \t") + 1);
			i++;
		}

		string op = tokens[0];
		if (skip) {
			if (op == "EndHeader") skip = false;
		} else {
			if (op == "DiskRead" || op == "DiskWrite") {
				unsigned long long offset = 0;
				unsigned long length = 0, disknum = 0;
				try {
					offset = stoull(tokens[5], nullptr, 16);
					length = stoul(tokens[6], nullptr, 16);
					disknum = stoul(tokens[8], nullptr, 10);
				} catch(exception& e) {
					cout << e.what() << endl;
				}
				unsigned long long block_id = offset >> block_scale;
				for (auto n: neighbours) cout << n << "\t" << block_id << endl;
				neighbours.push_front(block_id);
				if (neighbours.size() > diffuse_factor) neighbours.pop_back();
			}
		}
	}

	return 0;
}
