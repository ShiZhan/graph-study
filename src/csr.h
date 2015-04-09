#ifndef csr_h
#define csr_h
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include "utils.h"

#define ADDR(X) (X << 3) // get vertex address, multiply by sizeof(uint64_t).
#define SIZE(X) (X << 3) // get neighbour size, multiply by sizeof(uint64_t).

class CSR
{
public:
	CSR() { loaded = false; vertices = NULL; }
	~CSR() { unload(); }

	uint64_t load(std::string prefix) {
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
				uint64_t size = ri.tellg();
				vertices = (uint64_t*)malloc((size_t)size);
				ri.seekg(0, std::ios::beg);
				ri.read((char*)vertices, size);
				ri.close();
				loaded = true;
				return size/sizeof(uint64_t)-1;
			}
		} else return 0;
	}

	void unload() {
		if (loaded) {
			free(vertices);
			vertices = NULL;
			ci.close();
            loaded = false;
		}
	}

	uint64_t getNeighbours(uint64_t source, std::vector<uint64_t> &neighbours) {
		if (loaded) {
			uint64_t row_begin = vertices[source];
			uint64_t row_end   = vertices[source+1];
			uint64_t total = row_end - row_begin; // ASSERT total < 2^25
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
	uint64_t* vertices;
};
#endif
