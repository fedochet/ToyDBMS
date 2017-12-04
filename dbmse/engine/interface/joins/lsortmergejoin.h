#pragma once

#include "../interface.h"

struct LSortMergeJoin : public LAbstractNode {
    LSortMergeJoin(LAbstractNode* left, LAbstractNode* right, std::string left_offset, std::string right_offset);

private:
    std::string left_offset;
    std::string right_offset;

};
