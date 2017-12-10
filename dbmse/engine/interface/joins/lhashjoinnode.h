#include "labstractjoinnode.h"

struct LHashJoinNode : LAbstractJoinNode {
    LHashJoinNode(LAbstractNode* left,
                  LAbstractNode* right,
                  const std::string &left_offset,
                  const std::string &right_offset);

};