#include "histogram.h"

using namespace std;

void test_histograms() {
    vector<int> data = {10, 5, 16, 94, 33, 82, 11, 45};
    vector<int> sortedData = data, sort(begin(data), end(data));
    Histogram<int> h(data);
}

int main() {
    test_histograms();
    return 0;
}