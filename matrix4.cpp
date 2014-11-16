#include <iostream>
#include <math.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/random.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::numeric::ublas;
using namespace boost::random;
using namespace boost::program_options;

int main (int argc, char* argv[]) {
    int scale = 8, depth = 3;

    try {
        options_description opts("options");
        opts.add_options()
            ("help,h", "help message")
            ("scale,S", value<int>()->implicit_value(8), "2^scale verticies")
            ("depth,D", value<int>()->implicit_value(3), "depth < scale, the layers down to motif");
    
        variables_map vm;
        store(parse_command_line(argc, argv, opts), vm);
        notify(vm);

        if (vm.size() == 0) {
            cout << opts << endl;
            return 0;
        }

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
        return -1;
    }

    if (scale <= depth) {
        cout << "scale <= depth!" << endl;
        return -1;
    }

    int motif_scale = scale - depth;
    int result_size = pow(2, scale);
    int motif_size  = pow(2, motif_scale);
    int motif_mask  = motif_size - 1;

    cout << result_size << endl;

    boost::mt19937 rng;
    boost::uniform_int<> binary(0, 1);

    matrix<int> motif(motif_size, motif_size);

    for (unsigned i = 0; i < motif_size; ++ i)
        for (unsigned j = 0; j < motif_size; ++ j)
            motif(i, j) = binary(rng);

    cout << motif << endl;

    auto kronecker_cell = [=](unsigned i, unsigned j) {
        int cell = 1;
        unsigned motif_i = i, motif_j = j;
        for (unsigned d = 0; d < depth + 1; ++ d) {
            cell *= motif(motif_i & motif_mask, motif_j & motif_mask);
            motif_i >>= motif_scale;
            motif_j >>= motif_scale;
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
