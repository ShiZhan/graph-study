#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std; 
using namespace boost::numeric::ublas;

int main () {
    matrix<complex<double>> m(3, 3);
    for (unsigned i = 0; i < m.size1(); ++ i)
        for (unsigned j = 0; j < m.size2(); ++ j)
            m(i, j) = complex<double> (3 * i + j, 3 * i + j);

    cout << - m << endl;
    cout << conj (m) << endl;
    cout << real (m) << endl;
    cout << imag (m) << endl;
    cout << trans (m) << endl;
    cout << herm (m) << endl;
}
