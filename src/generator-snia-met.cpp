#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

int main (int argc, char* argv[]) {
	using namespace std;

	int diffuse = 0;
	string filename;

	if (argc < 3) {
		cerr << "generator <trace.csv> <diffuse factor>" << endl;
		return -1;
	}

	try {
		filename = argv[1];
        size_t lastChar;
        diffuse = stoi(argv[2], &lastChar, 10);
	} catch(exception& e) {
		cout << e.what() << endl;
		return -1;
	}

	ifstream infile(filename);
	if (!infile) return -1;

    string line;
    vector<string> tokens;

	while (getline(infile, line)) {
		stringstream ss(line);
		string token;
		tokens.clear();
        while(getline(ss, token, ',')) tokens.push_back(token);

		if (tokens.size() < 15) continue;

		for (auto token: tokens) cout << token << '|';

		cout << "\n----------------------" << endl;
	}

	return 0;
}
