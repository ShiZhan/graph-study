#include <iostream>
#include <string>

#include <boost/graph/adjacency_list.hpp>

#include "options.h"
#include "utils.h"

int main (int argc, char* argv[]) {
	using namespace std;
	using namespace boost;
	using namespace opt;

	const char* usage =
		"streaming-sampling [options]\n"
		" -h:\t ask for help\n"
		" -s:\t sampling policy\n"
		" -l:\t output log instead of subgraph\n"
		"\n";

	if (chkOption(argv, argv + argc, "-h")) {
		cout << usage;
		return 0;
	}

	char* sampling_p = getOption(argv, argv + argc, "-s");
	bool  output_log = chkOption(argv, argv + argc, "-l");

	typedef adjacency_list<setS, vecS, undirectedS, no_property> graph_t;

	graph_t g;
	string line;
	uint u, v;

	while (getline(cin, line)) {
		SSCANF((line.c_str(), "%u %u", &u, &v));
		if (output_log) cout << "a " << u << " " << v << endl;
		add_edge(u, v, g);
	}

	if (!output_log) {
		graph_t::edge_iterator ei, ei_end;
		for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
			cout << source(*ei, g) << " " << target(*ei, g) << endl;
	}

	return 0;
}
