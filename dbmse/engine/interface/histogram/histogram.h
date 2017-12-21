#pragma once

#include <map>
#include <algorithm>
#include <functional>
#include <iostream>

struct Selectivity {
    size_t lower_bound;
    size_t upper_bound;
    bool operator==(const Selectivity& other) {
        return lower_bound == other.lower_bound
               && upper_bound == other.upper_bound;
    }
};

template<class T>
struct Histogram {
    explicit Histogram(std::vector<T> data,
                       std::function<bool(const T &, const T &)> ascending_ordering = std::less_equal<T>(),
                       size_t number_of_buckets = 10);

    Selectivity CountBelowValue(const T &val);

private:

    struct BucketInfo {
        T val;
        size_t count;
    };

    size_t total_records;
    std::function<bool(const T &, const T &)> ordering;
    std::vector<BucketInfo> bucket_infos;
};


template<class T>
Histogram<T>::Histogram(std::vector<T> data, std::function<bool(const T &, const T &)> ascending_ordering,
                        size_t number_of_buckets)
    : total_records(data.size()), ordering(ascending_ordering) {

    struct Bucket : private std::vector<T> {
        using std::vector<T>::push_back;
        using std::vector<T>::empty;
        using std::vector<T>::size;
        using std::vector<T>::back;
    };

    std::vector<Bucket> buckets(number_of_buckets, Bucket());

    if (number_of_buckets == 0) {
        throw std::runtime_error("Cannot have a histogram with zero bucket_infos!");
    }

    std::sort(std::begin(data), std::end(data), ordering);
    double records_per_bucket = static_cast<double>(data.size()) / static_cast<double>(number_of_buckets);

    for (size_t i = 0; i < total_records; i++) {
        auto bucket_number = static_cast<size_t> (static_cast<double>(i + 1) / records_per_bucket) - 1;
        buckets.at(bucket_number).push_back(data.at(i));
    }

    auto to_remove_from = std::remove_if(std::begin(buckets), std::end(buckets), [](Bucket &b) { return b.empty(); });
    buckets.erase(to_remove_from, buckets.end());

    for (Bucket &b: buckets) {
        size_t previous_count = b.size();

        if (!bucket_infos.empty()) {
            previous_count += bucket_infos.back().count;
        }

        BucketInfo info = {b.back(), previous_count};
        bucket_infos.push_back(info);
    }
}

template<class T>
Selectivity Histogram<T>::CountBelowValue(const T &val) {

    auto first_greater = std::find_if(std::begin(bucket_infos), std::end(bucket_infos), [&](BucketInfo &b) {
        return !ordering(b.val, val);
    });

    if (first_greater == std::begin(bucket_infos)) {
        return Selectivity {0, first_greater->count};
    }

    if (first_greater == std::end(bucket_infos)) {
        return Selectivity {total_records, total_records};
    }

    return Selectivity {(first_greater - 1)->count, first_greater->count};

}
