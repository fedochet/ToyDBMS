#pragma once

#include "../interface.h"

struct LSortMergeJoin : public LAbstractNode {
    LSortMergeJoin(LAbstractNode* left, LAbstractNode* right, std::string left_offset, std::string right_offset);

    ~LSortMergeJoin() override;

private:
    std::string left_offset;
    std::string right_offset;

    void AssertColumnsSortedSame(size_t left_column, size_t right_column);
};
