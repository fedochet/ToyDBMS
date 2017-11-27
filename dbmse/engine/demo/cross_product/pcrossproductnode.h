#pragma once
#include "../pgetnextnode.h"

class PCrossProductNode : public PGetNextNode {
public:
    PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LCrossProductNode* source);
    query_result GetNextBlock() override;

    size_t GetAttrNum() override;

    void Print(int indent) override;
    ~PCrossProductNode() override;

private:
    query_result right_node_table;
    query_result current_left_block;
    size_t current_left_pos;
    size_t current_right_pos;

    void UpdateLeftBlock();

    void LoadRightBlock();
};