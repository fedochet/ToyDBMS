#pragma once

#include <algorithm>

namespace utils {

    template<class T>
    void append_to_back(std::vector<T> &to, const std::vector<T> &from) {
      to.insert(std::end(to), std::begin(from), std::end(from));
    }

    template<class T>
    bool contains(std::vector<T> &in, const T &value) {
      return std::find(std::begin(in), std::end(in), value) != std::end(in);
    }

    template <class T, class U>
    size_t find(T& collection, U& elem) {
      return std::find(std::begin(collection), std::end(collection), elem) - std::begin(collection);
    };
}