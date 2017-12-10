//
// Created by roman on 10.12.17.
//

#include "lhashjoinnode.h"

LHashJoinNode::LHashJoinNode(LAbstractNode* left, LAbstractNode* right,
                             const std::string &left_offset,
                             const std::string &right_offset)
    : LAbstractJoinNode(left, right, left_offset, right_offset) {}
