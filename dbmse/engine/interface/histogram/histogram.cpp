#include <assert.h>
#include "histogram.h"

using namespace std;

void test_histograms() {
    vector<int> data = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    Histogram<int> h(data);

    assert((h.CountBelowValue(5)   == Selectivity {0, 1}));
    assert((h.CountBelowValue(10)  == Selectivity {1, 2}));
    assert((h.CountBelowValue(15)  == Selectivity {1, 2}));
    assert((h.CountBelowValue(20)  == Selectivity {2, 3}));
    assert((h.CountBelowValue(25)  == Selectivity {2, 3}));
    assert((h.CountBelowValue(30)  == Selectivity {3, 4}));
    assert((h.CountBelowValue(35)  == Selectivity {3, 4}));
    assert((h.CountBelowValue(40)  == Selectivity {4, 5}));
    assert((h.CountBelowValue(45)  == Selectivity {4, 5}));
    assert((h.CountBelowValue(50)  == Selectivity {5, 6}));
    assert((h.CountBelowValue(55)  == Selectivity {5, 6}));
    assert((h.CountBelowValue(60)  == Selectivity {6, 7}));
    assert((h.CountBelowValue(65)  == Selectivity {6, 7}));
    assert((h.CountBelowValue(70)  == Selectivity {7, 8}));
    assert((h.CountBelowValue(75)  == Selectivity {7, 8}));
    assert((h.CountBelowValue(80)  == Selectivity {8, 9}));
    assert((h.CountBelowValue(85)  == Selectivity {8, 9}));
    assert((h.CountBelowValue(90)  == Selectivity {9, 10}));
    assert((h.CountBelowValue(95)  == Selectivity {9, 10}));
    assert((h.CountBelowValue(100) == Selectivity {10, 10}));
    assert((h.CountBelowValue(105) == Selectivity {10, 10}));
    assert((h.CountBelowValue(110) == Selectivity {10, 10}));
}

void test_predicates_matching() {
    vector<int> data = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    Histogram<int> h(data);

    cout << h.CountStatisticsFor(10) << endl;
    cout << h.CountStatisticsFor(15) << endl;
    cout << h.CountStatisticsFor(20) << endl;
    cout << h.CountStatisticsFor(100) << endl;
}


template class Histogram<int>;

int main() {
    test_histograms();
    test_predicates_matching();

    return 0;
}