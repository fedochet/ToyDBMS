#pragma once

#include "../interface.h"

class LJoinNode : public LAbstractNode {
public:
    // offsets are defined as "TableName.AttributeName" so, ensure there is no duplicates
    LJoinNode(LAbstractNode* left, LAbstractNode* right, std::string offset1, std::string offset2, int memorylimit = 0);
    ~LJoinNode();
    // attributes to perform equi-joins on
    std::string offset1, offset2;
    // maximum number of records permitted to present inside physical node
    int memorylimit;
};