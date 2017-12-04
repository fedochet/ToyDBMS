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
    utils::BlockIterator left_iterator;
    utils::BlockIterator right_iterator;

    size_t left_join_offset;
    size_t right_join_offset;
    size_t FindColumnOffset(const std::vector<std::vector<std::string>> &names) const;

};
