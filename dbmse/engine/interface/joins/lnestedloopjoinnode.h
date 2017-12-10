#pragma once

#include "../interface.h"
#include "../../utils/utils.h"
#include "labstractjoinnode.h"

class LNestedLoopJoinNode : public LAbstractJoinNode {
public:
    // offsets are defined as "TableName.AttributeName" so, ensure there is no duplicates
    LNestedLoopJoinNode(LAbstractNode* left, LAbstractNode* right, std::string offset1, std::string offset2);
};