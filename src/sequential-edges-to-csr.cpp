#include <iostream>
#include <string>
#include <fstream>
#include "options.h"

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	typedef unsigned long long ID;

	// default values
	string prefix = "csr";

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "sequential-edges-to-csr -flag [option]" << endl;
		cout << " -h:\t ask for help" << endl;
		cout << " -p:\t (" << prefix  << ") file name prefix for column and row index" << endl;
		return 0;
	}

	char* prefix_str  = getOption(argv, argv + argc, "-p");

	// get prefix
	if (prefix_str) prefix = prefix_str;
	string col_index_name = prefix + "_col.idx";
	string row_index_name = prefix + "_row.idx";

	// prepare output indices
	ofstream ci(col_index_name, ios::binary), ri(row_index_name, ios::binary);

	// write v sequence to ci, count u run length and write to ri.
	string line;
	ID u = 0, v = 0, u_prev = 0, offset = 0;
	ri.write((char *)&offset, sizeof(ID));
	while (getline(cin, line)) {
#ifdef _MSC_VER
		sscanf_s(line.c_str(), "%llu %llu", &u, &v);
#else
		sscanf(line.c_str(), "%llu %llu", &u, &v);
#endif
		ci.write((char *)&v, sizeof(ID));
		if(u > u_prev) {
			for (int i=0; i<(u-u_prev); i++) ri.write((char *)&offset, sizeof(ID));
			u_prev = u;
		}
		offset++;
	}
	ri.write((char *)&offset, sizeof(ID));

	ci.close();
	ri.close();

	cout << "vertices (" << row_index_name << "): " << u << endl;
	cout << "edges    (" << col_index_name << "): " << offset << endl;

	return 0;
}
