#pragma once

#include "../interface.h"
#include "labstractjoinnode.h"

struct LSortMergeJoinNode : LAbstractJoinNode {

    LSortMergeJoinNode(LAbstractNode* left, LAbstractNode* right, std::string left_offset, std::string right_offset);

private:
    void AssertColumnsSortedSame(size_t left_column, size_t right_column);
};
