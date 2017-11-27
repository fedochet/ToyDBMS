#pragma once

#include <cstddef>
#include "utils.h"
#include "../demo/pgetnextnode.h"

namespace utils {
    template<class T, class U, class K>
    T GetNextBlock(size_t &left_pos, T &left_block,
                   size_t &right_pos, T &right_table,
                   U update_block, K update_result) {
      query_result result_block;
      while (result_block.size() < BLOCK_SIZE
             && !right_table.empty()
             && !left_block.empty()) {

        if (right_pos >= right_table.size()) {
          right_pos = 0;
          left_pos++;
        }

        if (left_pos >= left_block.size()) {
          update_block();
          continue;
        }

        for (; right_pos < right_table.size(); right_pos++) {
          update_result(result_block, left_block[left_pos], right_table[right_pos]);
          if (result_block.size() >= BLOCK_SIZE) {
            right_pos++;
            break;
          }
        }
      }

      return result_block;

    };
}