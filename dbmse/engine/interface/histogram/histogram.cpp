#include <assert.h>
#include "histogram.h"

using namespace std;

void test_histograms() {
    vector<int> data = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    Histogram<int> h(data);

    assert((h.CountBelowValue(5) == std::make_pair<size_t, size_t>(0, 1)));
    assert((h.CountBelowValue(10) == std::make_pair<size_t, size_t>(1, 2)));
    assert((h.CountBelowValue(15) == std::make_pair<size_t, size_t>(1, 2)));
    assert((h.CountBelowValue(20) == std::make_pair<size_t, size_t>(2, 3)));
    assert((h.CountBelowValue(25) == std::make_pair<size_t, size_t>(2, 3)));
    assert((h.CountBelowValue(30) == std::make_pair<size_t, size_t>(3, 4)));
    assert((h.CountBelowValue(35) == std::make_pair<size_t, size_t>(3, 4)));
    assert((h.CountBelowValue(40) == std::make_pair<size_t, size_t>(4, 5)));
    assert((h.CountBelowValue(45) == std::make_pair<size_t, size_t>(4, 5)));
    assert((h.CountBelowValue(50) == std::make_pair<size_t, size_t>(5, 6)));
    assert((h.CountBelowValue(55) == std::make_pair<size_t, size_t>(5, 6)));
    assert((h.CountBelowValue(60) == std::make_pair<size_t, size_t>(6, 7)));
    assert((h.CountBelowValue(65) == std::make_pair<size_t, size_t>(6, 7)));
    assert((h.CountBelowValue(70) == std::make_pair<size_t, size_t>(7, 8)));
    assert((h.CountBelowValue(75) == std::make_pair<size_t, size_t>(7, 8)));
    assert((h.CountBelowValue(80) == std::make_pair<size_t, size_t>(8, 9)));
    assert((h.CountBelowValue(85) == std::make_pair<size_t, size_t>(8, 9)));
    assert((h.CountBelowValue(90) == std::make_pair<size_t, size_t>(9, 10)));
    assert((h.CountBelowValue(95) == std::make_pair<size_t, size_t>(9, 10)));
    assert((h.CountBelowValue(100) == std::make_pair<size_t, size_t>(10, 10)));
    assert((h.CountBelowValue(105) == std::make_pair<size_t, size_t>(10, 10)));
    assert((h.CountBelowValue(110) == std::make_pair<size_t, size_t>(10, 10)));
}

int main() {
    test_histograms();
    return 0;
}