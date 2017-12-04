#pragma once

#include <cstddef>
#include "utils.h"
#include "../demo/pgetnextnode.h"

namespace utils {
    struct BlockIterator {
        BlockIterator(PGetNextNode* node);
        const query_result_row* operator* () const;
        const query_result_row&operator->() const;
        BlockIterator& operator++();
        void Rewind();
        bool Closed() const;

    private:
        PGetNextNode* node;
        size_t current_pos;
        query_result current_block;
    };
}