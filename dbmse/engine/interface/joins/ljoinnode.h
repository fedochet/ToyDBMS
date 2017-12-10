#pragma once

#include "../interface.h"
#include "../../utils/utils.h"
#include "labstractjoinnode.h"

class LJoinNode : public LAbstractJoinNode {
public:
    // offsets are defined as "TableName.AttributeName" so, ensure there is no duplicates
    LJoinNode(LAbstractNode* left, LAbstractNode* right, std::string offset1, std::string offset2);
};