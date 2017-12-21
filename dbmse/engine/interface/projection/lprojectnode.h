#pragma once

#include "../interface.h"

class LProjectNode : public LAbstractNode {
public:
    // offsets to keep
    LProjectNode(LAbstractNode* child, std::vector<std::string> tokeep);

    // offsets are defined as "TableName.AttributeName" so, ensure there is no duplicates
    std::vector<size_t> offsets;
};
