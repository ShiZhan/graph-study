#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std; 
namespace ublas = boost::numeric::ublas;

int main() {
    ublas::vector<double> v1(3), v2(3);
    for (unsigned i = 0; i < 3; ++i)
        v1 (i) = i, v2 (i) = i*10;

    cout << v1 << endl
        << v2 << endl
        << outer_prod(v1, v2) << endl;
}
