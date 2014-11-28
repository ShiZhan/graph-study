#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>

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
    vector<string> tokens;
	list<unsigned long long> neighbours;
	bool skip = true;
	while (getline(infile, line)) {
		stringstream splitter(line);
		string token;
		tokens.clear();
        while(getline(splitter, token, ','))
			tokens.push_back(token.erase(0, token.find_last_of(" \t") + 1));

		string op = tokens.at(0);
		if (skip) {
			if (op == "EndHeader") skip = false;
		} else {
			if ((op == "DiskRead" || op == "DiskWrite") && tokens.size() == 15) {
				unsigned long long offset = 0;
				unsigned long length = 0, disknum = 0;
				try {
					offset = stoull(tokens.at(5), nullptr, 16);
					length = stoul(tokens.at(6), nullptr, 16);
					disknum = stoul(tokens.at(8), nullptr, 10);
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
