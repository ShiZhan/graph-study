#include <iostream>
#include <sstream>
#include <list>
#include <string>

#define MAX_TOKENS 255

int main (int argc, char* argv[]) {
	using namespace std;

	unsigned short diffuse_factor = 0;
	unsigned short block_scale = 16;

	if (argc < 3) {
		cerr << "generator <diffuse factor> <block scale>" << endl;
		return -1;
	}

	try {
		size_t lastChar;
		diffuse_factor = stoi(argv[1], &lastChar, 10);
		block_scale = stoi(argv[2], &lastChar, 10);
	} catch(exception& e) {
		cout << e.what() << endl;
		return -1;
	}

    string line;
	string *tokens = new string[MAX_TOKENS];
	list<unsigned long long> neighbours;
	while (getline(cin, line)) {
		stringstream splitter(line);
		string token;
		int i = 0;
		while(getline(splitter, token, ',')) {
			tokens[i] = token.erase(0, token.find_first_not_of(" \t"));
			if (i < MAX_TOKENS-1) i++;
		}

		string op = tokens[0];
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

	return 0;
}
