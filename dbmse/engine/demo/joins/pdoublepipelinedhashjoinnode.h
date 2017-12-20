#pragma once


#include <map>
#include "../pgetnextnode.h"
#include "../../interface/joins/lhashjoinnode.h"
#include "joinutils.h"
#include "../../utils/bd_utils.h"

struct PDoublePipelinedHashJoinNode : PGetNextNode {
    PDoublePipelinedHashJoinNode(LDoublePipelinedHashJoin* p, PGetNextNode* left, PGetNextNode* right);

    size_t GetAttrNum() override;

    void Rewind() override;

    query_result GetNextBlock() override;

    void Print(size_t indent) override;

    ~PDoublePipelinedHashJoinNode() override;

private:
    join_offset left_join_offset;
    join_offset right_join_offset;

    utils::TableRowMerger merger;
    utils::BlockIterator left_iterator;
    utils::BlockIterator right_iterator;

    std::map<Value, query_result> left_hash_table;
    std::map<Value, query_result> right_hash_table;

    bool from_left{true};
    size_t right_block_pos{0};
    size_t left_block_pos{0};
};
