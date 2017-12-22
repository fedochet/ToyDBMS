#pragma once

#include <map>
#include <algorithm>
#include <functional>
#include <iostream>

struct Statistics {
    size_t less_than;
    size_t greater_than;
    size_t equals;
};

inline std::ostream& operator<<(std::ostream& os, const Statistics& c) {
    os << "less than: " << c.less_than
      << ", equals: " << c.equals
      << ", greater_than: " << c.greater_than << ", totaling " << c.greater_than + c.equals + c.less_than;

    return os;
}

template<class T>
struct Histogram {

    explicit Histogram(std::vector<T> data,
                       std::function<bool(const T &, const T &)> ascending_ordering = std::less<T>(),
                       size_t number_of_buckets = 10);

    Statistics CountStatisticsFor(const T &val) const;

private:

    struct BucketInfo {
        T left_val;
        T right_val;
        size_t in_bucket;
    };

    size_t total_records;
    std::function<bool(const T &, const T &)> less_predicate;
    std::vector<BucketInfo> bucket_infos;

    bool less(const T& left, const T& right) const {
        return less_predicate(left, right);
    }

    bool leq(const T& left, const T& right) const {
        return less(left, right) || equal(left, right);
    }

    bool equal(const T& left, const T& right) const {
        return !less(left, right) && !less(right, left);
    }

};


template<class T>
Histogram<T>::Histogram(std::vector<T> data, std::function<bool(const T &, const T &)> ascending_ordering,
                        size_t number_of_buckets)
    : total_records(data.size()), less_predicate(ascending_ordering) {

    struct Bucket : private std::vector<T> {
        using std::vector<T>::push_back;
        using std::vector<T>::empty;
        using std::vector<T>::size;
        using std::vector<T>::back;
        using std::vector<T>::front;
    };

    std::vector<Bucket> buckets(number_of_buckets, Bucket());

    if (number_of_buckets == 0) {
        throw std::runtime_error("Cannot have a histogram with zero bucket_infos!");
    }

    std::sort(std::begin(data), std::end(data), less_predicate);
    double records_per_bucket = static_cast<double>(data.size()) / static_cast<double>(number_of_buckets);

    for (size_t i = 0; i < total_records; i++) {
        auto bucket_number = static_cast<size_t>(ceil(static_cast<double>(i + 1) / records_per_bucket)) - 1;
        buckets.at(bucket_number).push_back(data.at(i));
    }

    auto to_remove_from = std::remove_if(std::begin(buckets), std::end(buckets), [](Bucket &b) { return b.empty(); });
    buckets.erase(to_remove_from, buckets.end());

    for (Bucket &b: buckets) {
        size_t previous_count = b.size();
        BucketInfo info = {b.front(), b.back(), previous_count};
        bucket_infos.push_back(info);
    }
}

template<class T>
Statistics Histogram<T>::CountStatisticsFor(const T &val) const {

    Statistics result {0,0,0};

    for (const BucketInfo& bucket: bucket_infos) {
        if (less(bucket.right_val, val)) {
            result.less_than += bucket.in_bucket;
            continue;
        }

        if (leq(bucket.left_val, val) && leq(val, bucket.right_val)) {
            result.equals += bucket.in_bucket;
            continue;
        }

        if (less(val, bucket.left_val)) {
            result.greater_than += bucket.in_bucket;
            continue;
        }
    }

    return result;
}
