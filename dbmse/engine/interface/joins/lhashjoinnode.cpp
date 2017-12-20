#include "lhashjoinnode.h"

template <typename T>
LHashJoinNode<T>::LHashJoinNode(LAbstractNode* left, LAbstractNode* right,
                             const std::string &left_offset,
                             const std::string &right_offset)
    : LAbstractJoinNode(left, right, left_offset, right_offset) {}
