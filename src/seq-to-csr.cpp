#include <iostream>
#include <string>
#include <fstream>
#include "options.h"
#include "utils.h"

#define PREFIX "csr" // CSR indices name prefix default

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "seq-to-csr -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -p:\t (" << PREFIX << ") file name prefix for column and row index" << endl;
		return 0;
	}

	char* prefix_str = getOption(argv, argv + argc, "-p");

	// get prefix
	string prefix = prefix_str ? prefix_str : PREFIX;
	string col_index_name = prefix + "-col.idx";
	string row_index_name = prefix + "-row.idx";

	// prepare output indices
	ofstream ci(col_index_name, ios::binary), ri(row_index_name, ios::binary);

	// write v sequence to ci, count u run length and write to ri.
	string line;
	u64 u = 0, v = 0, u_prev = 0, offset = 0;
	ri.write((char *)&offset, sizeof(u64));
	while (getline(cin, line)) {
		SSCANF((line.c_str(), "%llu %llu", &u, &v));
		ci.write((char *)&v, sizeof(u64));
		if(u > u_prev) {
			for (int i=0; i<(u-u_prev); i++) ri.write((char *)&offset, sizeof(u64));
			u_prev = u;
		}
		offset++;
	}
	ri.write((char *)&offset, sizeof(u64));

	ci.close();
	ri.close();

	cout << "vertices (" << row_index_name << "): " << ++u << endl;
	cout << "edges    (" << col_index_name << "): " << offset << endl;

	return 0;
}
