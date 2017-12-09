#pragma once

#include "../interface.h"

struct LSortMergeJoinNode : public LAbstractNode {

    LSortMergeJoinNode(LAbstractNode* left, LAbstractNode* right, std::string left_offset, std::string right_offset);

    ~LSortMergeJoinNode() override;
    join_offset GetLeftOffset();
    join_offset GetRightOffset();

private:
    void AssertColumnsSortedSame(size_t left_column, size_t right_column);
    size_t FindColumnOffset(const std::vector<std::vector<std::string>> &names) const;

private:
    std::string left_offset;
    std::string right_offset;

};
