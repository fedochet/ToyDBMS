#include "psortmergejoinnode.h"

using namespace std;

PSortMergeJoinNode::PSortMergeJoinNode(LSortMergeJoinNode* p, PGetNextNode* left, PGetNextNode* right)
    : PGetNextNode(p, left, right),
      left_join_offset(p->GetLeftOffset()),
      right_join_offset(p->GetRightOffset()),
      left_iterator(left),
      right_iterator(right, right_join_offset.first) {}

PSortMergeJoinNode::~PSortMergeJoinNode() {
  delete left;
  delete right;
}

void PSortMergeJoinNode::Print(size_t indent) {
  for (size_t i = 0; i < indent; i++) {
    cout << " ";
  }
  cout << "NL-JOIN: "
       << left_join_offset.second
       << "="
       << right_join_offset.second
       << endl;
  left->Print(indent + 2);
  right->Print(indent + 2);

}

query_result PSortMergeJoinNode::GetNextBlock() {
  query_result result_block;
  while (result_block.size() < BLOCK_SIZE && !left_iterator.Closed() && !right_iterator.Closed()) {
    auto &left_row = *left_iterator;
    auto &right_rows = *right_iterator;

    if (left_row[left_join_offset.first] < right_rows.back()[right_join_offset.first]) {
      ++left_iterator;
      continue;
    }

    if (right_rows.back()[right_join_offset.first] < left_row[left_join_offset.first]) {
      ++right_iterator;
      continue;
    }

    if (left_row[left_join_offset.first] == right_rows.back()[right_join_offset.first]) {

      while (result_block.size() < BLOCK_SIZE && right_block_pos < right_rows.size()) {

        query_result_row tmp;
        for (size_t k = 0; k < left->prototype->fieldNames.size(); k++) {
          if (k != left_join_offset.first) {
            tmp.push_back(left_row[k]);
          }
        }

        for (size_t k = 0; k < right->prototype->fieldNames.size(); k++) {
          if (k != right_join_offset.first) {
            tmp.push_back(right_rows[right_block_pos][k]);
          }
        }

        tmp.push_back(left_row[left_join_offset.first]);
        result_block.push_back(tmp);

        right_block_pos++;
      }

      if (right_block_pos >= right_rows.size()) {
        right_block_pos = 0;
        ++left_iterator;
      }
    }

  }

  return result_block;

}

void PSortMergeJoinNode::Rewind() {
  left_iterator.Rewind();
  right_iterator.Rewind();
}

size_t PSortMergeJoinNode::GetAttrNum() {
  return PGetNextNode::GetAttrNum();
}
