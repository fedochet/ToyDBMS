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
    utils::BlockIterator left_iterator;
    utils::BlockIterator right_iterator;
};