#pragma once
#include "../pgetnextnode.h"
#include "../../interface/crossproduct/lcrossproductnode.h"
#include "../../utils/bd_utils.h"

class PCrossProductNode : public PGetNextNode {
public:
    PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LCrossProductNode* source);
    query_result GetNextBlock() override;
    void Rewind() override;

    size_t GetAttrNum() override;
    void Print(size_t indent) override;

    ~PCrossProductNode() override;

private:
    query_result current_right_block;
    size_t current_right_pos;

    utils::BlockIterator left_iterator;

    void UpdateRightBlock();
};