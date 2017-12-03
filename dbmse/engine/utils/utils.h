#pragma once

#include <algorithm>
#include <istream>
#include <string>

namespace utils {

    template<class T>
    void append_to_back(std::vector<T> &to, const std::vector<T> &from) {
      to.insert(std::end(to), std::begin(from), std::end(from));
    }

    template<class T>
    bool contains(std::vector<T> &in, const T &value) {
      return std::find(std::begin(in), std::end(in), value) != std::end(in);
    }

    template<class T, class U>
    size_t find(T &collection, U &elem) {
      return std::find(std::begin(collection), std::end(collection), elem) - std::begin(collection);
    };

    inline size_t skip_lines(std::istream &in, size_t n) {
      size_t skipped = 0;
      std::string temp;
      while (skipped != n && getline(in, temp)) {
        skipped ++;
      }

      return skipped;
    }
}