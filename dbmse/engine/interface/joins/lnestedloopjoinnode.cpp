#include "lnestedloopjoinnode.h"

using namespace utils;

LNestedLoopJoinNode::LNestedLoopJoinNode(LAbstractNode* left, LAbstractNode* right, std::string offset1, std::string offset2)
    : LAbstractJoinNode(left, right, offset1, offset2) {}