#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "options.h"
#include "utils.h"

#define PREFIX "csr" // CSR indices name prefix default
#define SOURCE 3     // source vertex default

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "sssp -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -p:\t (" << PREFIX << ") file name prefix for column and row index" << endl;
		cout << " -s:\t (" << SOURCE << ") source vertex" << endl;
		return 0;
	}

	char* prefix_str = getOption(argv, argv + argc, "-p");
	u64   source     = getU64(argv, argv + argc, "-s", SOURCE);

	// get prefix
	string prefix = prefix_str ? prefix_str : PREFIX;
	string col_index_name = prefix + "-col.idx";
	string row_index_name = prefix + "-row.idx";

	// prepare input indices
	ifstream ci(col_index_name, ios::binary), ri(row_index_name, ios::binary);
	if (!ci || !ri) {
		cerr << "CSR index files not exist!" << endl;
		ci.close(); ri.close();
		return -1;
	}

	// algorithm start
	u64 row_range[2] = {0, 0};
	u64 address = source << 3; // multiply by sizeof(u64)*2
	ri.seekg(address);
	ri.read((char*)row_range, sizeof(u64)*2);
	cout << row_range[0] << " " << row_range[1] << endl;
	u64 neighbour = 0;
	vector<u64> neighbours;
	u64 neighbour_total = row_range[1] - row_range[0];
	neighbours.resize((int)neighbour_total);
	ci.seekg(row_range[0] << 3);
	ci.read((char*)neighbours.data(), neighbour_total * sizeof(u64));
	for (u64 n: neighbours) cout << n << endl;
	// algorithm end

	ci.close();
	ri.close();

	return 0;
}
