#ifndef csr_h
#define csr_h
#include <iostream>
#include <fstream>
#include <string>
#include "utils.h"

#define ADDR(X) (X << 3) // get vertex address, multiply by sizeof(u64).
#define SIZE(X) (X << 3) // get neighbour size, multiply by sizeof(u64).

class CSR
{
public:
	CSR() { loaded = false; vertices = NULL; }
	~CSR() {}

	u64 load(std::string prefix) {
		if (!loaded) { // prepare input indices
			std::string col_index_name = prefix + "-col.idx";
			std::string row_index_name = prefix + "-row.idx";
			ci.open(col_index_name, std::ios::binary);
			ri.open(row_index_name, std::ios::binary);
			if (!ci || !ri) {
				std::cerr << "CSR index files not exist!" << std::endl;
				ci.close(); ri.close();
				return 0;
			} else {
				ri.seekg(0, std::ios::end);
				u64 size = ri.tellg();
				vertices = (u64*)malloc((size_t)size);
				ri.seekg(0, std::ios::beg);
				ri.read((char*)vertices, size);
				ri.close();
				loaded = true;
				return size-1;
			}
		} else return 0;
	}

	void unload() {
		if (loaded) {
			free(vertices);
			vertices = NULL;
			ci.close();
		}
	}

	u64 getNeighbours(u64 source, std::vector<u64> &neighbours) {
		if (loaded) {
			u64 row_begin = vertices[source];
			u64 row_end   = vertices[source+1];
			u64 total = row_end - row_begin; // ASSERT total < 2^25
			if (total) {
				neighbours.resize((unsigned int)total);
				ci.seekg(ADDR(row_begin));
				ci.read((char*)neighbours.data(), SIZE(total));
			}
			return total;
		} else return 0;
	}
private:
	std::ifstream ci, ri;
	bool loaded;
	u64* vertices;
};
#endif