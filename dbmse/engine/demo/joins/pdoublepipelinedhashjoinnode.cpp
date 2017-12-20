#include "pdoublepipelinedhashjoinnode.h"

using namespace std;

PDoublePipelinedHashJoinNode::PDoublePipelinedHashJoinNode(LDoublePipelinedHashJoin* p, PGetNextNode* left,
                                                           PGetNextNode* right)
    : PGetNextNode(p, left, right), left_join_offset(p->GetLeftOffset()),
      right_join_offset(p->GetRightOffset()),
      merger(p),
      left_iterator(left),
      right_iterator(right) {}


void PDoublePipelinedHashJoinNode::Print(size_t indent) {
    for (size_t i = 0; i < indent; i++) {
        cout << " ";
    }
    cout << "HASH-JOIN: "
         << left_join_offset.second
         << "="
         << right_join_offset.second
         << endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}

query_result PDoublePipelinedHashJoinNode::GetNextBlock() {
    return {};
}

void PDoublePipelinedHashJoinNode::Rewind() {
    left_iterator.Rewind();
    right_iterator.Rewind();
}

size_t PDoublePipelinedHashJoinNode::GetAttrNum() {
    return PGetNextNode::GetAttrNum();
}

PDoublePipelinedHashJoinNode::~PDoublePipelinedHashJoinNode() {
    delete left;
    delete right;
}