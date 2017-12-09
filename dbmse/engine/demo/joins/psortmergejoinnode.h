#pragma once

#include "../pgetnextnode.h"
#include "../../utils/bd_utils.h"
#include "../../interface/joins/lsortmergejoin.h"

struct PSortMergeJoinNode : PGetNextNode {
    PSortMergeJoinNode(LSortMergeJoinNode* p, PGetNextNode* left, PGetNextNode* right);

    ~PSortMergeJoinNode() override;

    void Print(size_t indent) override;

    query_result GetNextBlock() override;

    void Rewind() override;

    size_t GetAttrNum() override;

private:
    join_offset left_join_offset;
    join_offset right_join_offset;

    utils::BlockIterator left_iterator;
    utils::AdvanceBlockIterator right_iterator;
    size_t right_block_pos{0};

    query_result_row MergeRows(const query_result_row &left_row, const query_result_row &right_row) const;
};
