#pragma once

#include "../interface.h"

struct LSortMergeJoinNode : public LAbstractNode {
    LSortMergeJoinNode(LAbstractNode* left, LAbstractNode* right, std::string left_offset, std::string right_offset);

    ~LSortMergeJoinNode() override;

    std::string left_offset;
    std::string right_offset;

private:
    void AssertColumnsSortedSame(size_t left_column, size_t right_column);
};
