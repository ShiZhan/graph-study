#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "options.h"
#include "utils.h"

#define PREFIX "csr" // CSR indices name prefix default
#define SOURCE 3     // source vertex default

#define ADDR(X) (X << 3) // get vertex address, multiply by sizeof(u64).
#define SIZE(X) (X << 3) // get neighbour size, multiply by sizeof(u64).

u64 getNeighbours(std::ifstream &ci, std::ifstream &ri, u64 source, std::vector<u64> &neighbours) {
	u64 row_range[2] = {0, 0};
	u64 address = ADDR(source);
	ri.seekg(address);
	ri.read((char*)row_range, SIZE(2));
	u64 total = row_range[1] - row_range[0]; // ASSERT neighbour_total < 2^25
	neighbours.resize((int)total);
	ci.seekg(ADDR(row_range[0]));
	ci.read((char*)neighbours.data(), SIZE(total));
	return total;
}

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
	vector<u64> neighbours;
	u64 total = getNeighbours(ci, ri, source, neighbours);
	for (u64 n: neighbours) cout << n << endl;
	neighbours.clear();
	// algorithm end

	ci.close();
	ri.close();

	return 0;
}
