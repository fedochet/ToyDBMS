#pragma once

#include <cstddef>
#include "utils.h"
#include "../demo/pgetnextnode.h"

namespace utils {
    struct BlockIterator {
        explicit BlockIterator(PGetNextNode* node);

        const query_result_row &operator*() const;

        BlockIterator &operator++();

        void Rewind();

        bool Closed() const;

    private:
        PGetNextNode* node;
        size_t current_pos;
        query_result current_block;
    };

    struct AdvanceBlockIterator {
        explicit AdvanceBlockIterator(PGetNextNode* node, size_t offset);

        const query_result &operator*() const;

        AdvanceBlockIterator &operator++();

        void Rewind();

        bool Closed() const;

    private:
        BlockIterator iterator;
        query_result current_block;
        size_t offset;
    };

}