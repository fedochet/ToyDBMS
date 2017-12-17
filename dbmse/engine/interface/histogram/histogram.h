#pragma once

#include <map>
#include <algorithm>
#include <functional>

template<class T>
struct Histogram {
    explicit Histogram(std::vector<T> data,
                       std::function<bool(const T &, const T &)> ordering = std::less<T>(),
                       size_t number_of_buckets = 10);

private:
    struct Bucket : private std::vector<T> {
        using std::vector<T>::push_back;
        using std::vector<T>::empty;
    };

    size_t total_records;
    std::vector<Bucket> buckets;
};


template<class T>
Histogram<T>::Histogram(std::vector<T> data, std::function<bool(const T &, const T &)> ordering,
                        size_t number_of_buckets)
    : total_records(data.size())
    , buckets(number_of_buckets, Bucket()) {

    if (number_of_buckets == 0) {
        throw std::runtime_error("Cannot have a histogram with zero buckets!");
    }

    std::sort(std::begin(data), std::end(data), ordering);
    double records_per_bucket = data.size() / static_cast<double> (number_of_buckets);

    for (size_t i = 0; i < total_records; i++) {
        auto bucket_number = static_cast<size_t> ((i + 1) / records_per_bucket) - 1;
        buckets.at(bucket_number).push_back(data.at(i));
    }
}
