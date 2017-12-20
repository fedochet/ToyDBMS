#pragma once

#include "labstractjoinnode.h"

struct FullHashJoin {};
struct DoublePipelinedHashJoin {};

template <typename T = FullHashJoin>
struct LHashJoinNode : LAbstractJoinNode {
    LHashJoinNode(LAbstractNode* left,
                  LAbstractNode* right,
                  const std::string &left_offset,
                  const std::string &right_offset);

};

template struct LHashJoinNode<FullHashJoin>;
template struct LHashJoinNode<DoublePipelinedHashJoin>;

typedef LHashJoinNode<FullHashJoin> LFullHashJoin;
typedef LHashJoinNode<DoublePipelinedHashJoin> LDoublePipelinedHashJoin;
