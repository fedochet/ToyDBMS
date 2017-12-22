#include <assert.h>
#include <ctime>
#include "histogram.h"

using namespace std;

template class Histogram<int>;

int main() {
    std::srand(unsigned(std::time(0)));
    vector<int> test;
    for (size_t i = 0; i < 100000; i++) {
        test.push_back(std::rand() % 100);
    }

    Histogram<int> h(test, less<>(), 1024);

    int n;
    while (cin >> n) {
        cout << h.CountStatisticsFor(n) << endl;
    }

    return 0;
}