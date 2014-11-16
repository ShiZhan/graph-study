#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/random.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::numeric::ublas;
using namespace boost::random;
using namespace boost::program_options;

int main (int argc, char* argv[]) {
    int scale = 2, depth = 3;

    try {
        options_description opts("options");
        opts.add_options()
            ("help,h", "help message")
            ("scale,S", value<int>()->implicit_value(2), "2^scale motif verticies")
            ("depth,D", value<int>()->implicit_value(3), "recursive depth");
    
        variables_map vm;
        store(parse_command_line(argc, argv, opts), vm);
        notify(vm);

        if (vm.count("help")) {
            cout << opts << endl;
            return 0;
        }
    
        if (vm.count("scale")) {
            scale = vm["scale"].as<int>();
            cout << "scale opt: " << scale << endl;
        }
    
        if (vm.count("depth")) {
            depth = vm["depth"].as<int>();
            cout << "depth opt: " << depth << endl;
        }
    } catch(exception& e) {
        cout << e.what() << endl;
        cout << "use '-h' to get help." << endl;
        return -1;
    }

    int motif_size  = 1 << scale;
    int result_size = 1 << (scale * (depth + 1));
    int motif_mask  = motif_size - 1;

    cout << result_size << endl;

    boost::mt19937 rng;
    boost::uniform_int<> binary(0, 1);

    matrix<int> motif(motif_size, motif_size);

    for (unsigned i = 0; i < motif_size; ++ i)
        for (unsigned j = 0; j < motif_size; ++ j)
            motif(i, j) = (j < i) ? binary(rng) : 0;

    motif += trans(motif);

    cout << motif << endl;

    auto kronecker_cell = [=](unsigned i, unsigned j) {
        int cell = 1;
        unsigned motif_i = i, motif_j = j;
        for (unsigned d = 0; d < depth + 1; ++ d) {
            cell *= motif(motif_i & motif_mask, motif_j & motif_mask);
            motif_i >>= scale;
            motif_j >>= scale;
        }
        return cell;
    };

    for (unsigned i = 0; i < result_size; ++ i ) {
        for (unsigned j = 0; j < result_size; ++ j)
           cout << kronecker_cell(i, j) << ' ';
        cout << endl;
    }

    return 0;
}
