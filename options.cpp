#include <iostream>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

int print_vm(const options_description &opts, const variables_map &vm)
{
    if (vm.size() == 0) {
        cout << opts << endl;
        return 0;
    }

    if (vm.count("help")) {
        cout << opts << endl;
        return 0;
    }

    cout << "find opt: " << vm["filename"].as<string>() << endl;

    if (vm.count("dir")) {
        cout << "dir opt:";
        BOOST_FOREACH(string str, vm["dir"].as<vector<string> >()) {
            cout << str << ",";
        }
        cout << endl;
    }

    if (vm.count("depth")) {
        cout << "depth opt: " << vm["depth"].as<int>() << endl;
    }
}

int main(int argc, char* argv[])
{
    string filename;
    options_description opts("options");
    opts.add_options()
        ("help,h", "help message")
        ("filename,f", value<string>(&filename)->default_value("text"), "to find a file")
        ("dir,D", value<vector<string> >()->multitoken(), "search dir")
        ("depth,d", value<int>()->implicit_value(5), "search depth");

    variables_map vm;
    store(parse_command_line(argc, argv, opts), vm);
    notify(vm);
    print_vm(opts, vm);
}
