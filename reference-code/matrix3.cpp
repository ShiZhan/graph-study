#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace boost::numeric::ublas;
using namespace boost::random;

int main () {
    boost::mt19937 rng;
    boost::uniform_int<> binary(0, 1);

    matrix<int> m1(10, 10), m2(3, 3);

    int h1 = m1.size1(), w1 = m1.size2();
    int h2 = m2.size1(), w2 = m2.size2();
    int h  = h1 * h2,    w  = w1 * w2;

    for (unsigned i = 0; i < h1; ++ i)
        for (unsigned j = 0; j < w1; ++ j)
            m1(i, j) = binary(rng);

    for (unsigned i = 0; i < h2; ++ i)
        for (unsigned j = 0; j < w2; ++ j)
            m2(i, j) = binary(rng);

    cout << m1 << endl;
    cout << m2 << endl;

    for (unsigned i = 0; i < h; ++ i ) {
        for (unsigned j = 0; j < w; ++ j)
            cout << m1(i/h2, j/w2) * m2(i%h2, j%w2) << ' ';
        cout << endl;
    }

    return 0;
}
