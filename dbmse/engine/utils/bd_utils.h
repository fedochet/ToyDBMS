#pragma once

#include <cstddef>
#include "utils.h"
#include "../demo/pgetnextnode.h"

namespace utils {
    struct BlockIterator {
        explicit BlockIterator(PGetNextNode* node);

        virtual const query_result_row &operator*() const;

        virtual BlockIterator &operator++();

        virtual void Rewind();

        virtual bool Closed() const;

    private:
        PGetNextNode* node;
        size_t current_pos;
        query_result current_block;
    };

    struct CachedBlockIterator : BlockIterator {
        explicit CachedBlockIterator(PGetNextNode* node);

        CachedBlockIterator &operator++() override;

        CachedBlockIterator &RepeatCache();

        void Rewind() override;

        const query_result_row &operator*() const override;

        bool Closed() const override;

        void ClearCache();

    private:
        bool rewinded{false};
        query_result cache;
        size_t current_cache_pos{0};
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