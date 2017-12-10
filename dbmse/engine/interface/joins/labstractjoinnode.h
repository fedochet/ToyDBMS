#pragma once

#include "../interface.h"

struct LAbstractJoinNode : LAbstractNode {
    join_offset GetLeftOffset();

    join_offset GetRightOffset();

protected: // to enforce inheritance
    LAbstractJoinNode(LAbstractNode* left, LAbstractNode* right, std::string left_offset, std::string right_offset);

private:
    size_t FindColumnOffset(const std::vector<std::vector<std::string>> &names) const;

    std::string left_offset;
    std::string right_offset;

};
