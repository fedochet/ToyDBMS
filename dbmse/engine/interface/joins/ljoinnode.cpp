#include "ljoinnode.h"

using namespace utils;

LJoinNode::LJoinNode(LAbstractNode* left, LAbstractNode* right, std::string offset1, std::string offset2)
    : LAbstractJoinNode(left, right, offset1, offset2) {}