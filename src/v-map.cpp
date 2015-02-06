#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "options.h"
#include "utils.h"

template <typename K, typename V>
V getOrElse(const std::map<K, V>& m, const K k, const V d) {
	auto itr = m.find(k);
	if (itr == m.end())
		return d;
	else
		return itr->second;
}

typedef std::map<uint, uint> v_map_t;
void map_print(const char* line, const v_map_t& v_map) {
	uint u, v;
	SSCANF((line, "%u %u", &u, &v));
	printf("%u %u\n", getOrElse(v_map, u, u), getOrElse(v_map, v, v));
}

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "v-map [options]" << endl
			<< " -h:\t ask for help" << endl
			<< " -m:\t vertex mapping file" << endl
			<< " -i:\t get edges from (cin)" << endl;
		return 0;
	}

	char* v_map_file = getOption(argv, argv + argc, "-m");
	char* edges_file = getOption(argv, argv + argc, "-i");

	string line;
	v_map_t v_map;
	if (v_map_file) {
		ifstream map_fs(v_map_file);
		if(map_fs) {
			uint v_orig = 0, v_mapped = 0;
			while (getline(map_fs, line)) {
				SSCANF((line.c_str(), "%u", &v_orig));
				v_map[v_orig] = v_mapped;
				v_mapped++;
			}
			map_fs.close();
		}
	}

	if (edges_file) {
		ifstream edges_fs(edges_file);
		if(edges_fs) {
			while (getline(edges_fs, line)) map_print(line.c_str(), v_map);
			edges_fs.close();
		}
	} else  while (getline(cin, line)) map_print(line.c_str(), v_map);

	return 0;
}
