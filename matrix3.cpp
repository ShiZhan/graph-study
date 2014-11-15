#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace boost::numeric::ublas;
using namespace boost::random;

int main () {
    boost::mt19937 rng;
    boost::uniform_int<> hundred(0, 10);

    matrix<int> m1(10, 10), m2(3, 3);
    for (unsigned i = 0; i < m1.size1(); ++ i)
        for (unsigned j = 0; j < m1.size2(); ++ j)
            m1(i, j) = hundred(rng);

    for (unsigned i = 0; i < m2.size1(); ++ i)
        for (unsigned j = 0; j < m2.size2(); ++ j)
            m2(i, j) = hundred(rng);

    cout << m1 << endl;
    cout << m2 << endl;
    int h1 = m1.size1(), w1 = m1.size2();
    int h2 = m2.size1(), w2 = m2.size2();
    int h  = h1 * h2,    w  = w1 * w2;
    auto kronecker_product = [=](int i, int j){return m1(i/h2, j/w2) * m2(i%h2, j%w2);};

    for (unsigned i = 0; i < h; ++ i ) {
        for (unsigned j = 0; j < w; ++ j)
            cout << kronecker_product(i, j) << ' ';
        cout << endl;
    }

    return 0;
}

