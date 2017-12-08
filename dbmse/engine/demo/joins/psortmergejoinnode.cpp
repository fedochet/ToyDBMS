#include "psortmergejoinnode.h"

using namespace std;

PSortMergeJoinNode::PSortMergeJoinNode(LSortMergeJoinNode* p, PGetNextNode* left, PGetNextNode* right)
    : PGetNextNode(p, left, right),
      left_join_offset(FindColumnOffset(left->prototype->fieldNames)),
      right_join_offset(FindColumnOffset(right->prototype->fieldNames)),
      left_iterator(left),
      right_iterator(right, right_join_offset) {}

PSortMergeJoinNode::~PSortMergeJoinNode() {
  delete left;
  delete right;
}

void PSortMergeJoinNode::Print(size_t indent) {
  for (size_t i = 0; i < indent; i++) {
    cout << " ";
  }
  cout << "NL-JOIN: "
       << (dynamic_cast<LSortMergeJoinNode*>(prototype))->left_offset
       << "="
       << (dynamic_cast<LSortMergeJoinNode*>(prototype))->right_offset
       << endl;
  left->Print(indent + 2);
  right->Print(indent + 2);

}

query_result PSortMergeJoinNode::GetNextBlock() {
  PGetNextNode* l = dynamic_cast<PGetNextNode*>(left);
  PGetNextNode* r = dynamic_cast<PGetNextNode*>(right);
  LAbstractNode* lp = l->prototype;
  LAbstractNode* rp = r->prototype;
  vector<name_aliases> ln = lp->fieldNames;
  vector<name_aliases> rn = rp->fieldNames;

  query_result result_block;
  while (result_block.size() < BLOCK_SIZE && !left_iterator.Closed() && !right_iterator.Closed()) {
    auto &left_row = *left_iterator;
    auto &right_row = *right_iterator;

    if (left_row[left_join_offset] < right_row.back()[right_join_offset]) {
      ++left_iterator;
      continue;
    }

    if (right_row.back()[right_join_offset] < left_row[left_join_offset]) {
      ++right_iterator;
      continue;
    }

    if (left_row[left_join_offset] == right_row.back()[right_join_offset]) {

      while (result_block.size() < BLOCK_SIZE && right_block_pos < right_row.size()) {

        query_result_row tmp;
        for (size_t k = 0; k < ln.size(); k++) {
          if (k != left_join_offset) {
            tmp.push_back(left_row[k]);
          }
        }

        for (size_t k = 0; k < rn.size(); k++) {
          if (k != right_join_offset) {
            tmp.push_back(right_row[right_block_pos][k]);
          }
        }

        tmp.push_back(left_row[left_join_offset]);
        result_block.push_back(tmp);

        right_block_pos++;
      }

      if (right_block_pos >= right_row.size()) {
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

size_t PSortMergeJoinNode::FindColumnOffset(const std::vector<std::vector<std::string>> &names) const {
  auto offset_name_1 = (dynamic_cast<LSortMergeJoinNode*>(prototype))->left_offset;
  auto offset_name_2 = (dynamic_cast<LSortMergeJoinNode*>(prototype))->right_offset;

  for (size_t i = 0; i < names.size(); i++) {
    size_t lpos1 = utils::find(names[i], offset_name_1);
    size_t lpos2 = utils::find(names[i], offset_name_2);

    if (lpos1 < names[i].size() || lpos2 < names[i].size()) {
      return i;
    }
  }

  throw runtime_error(
      string("Cannot joins by column named ") + offset_name_1 + " or " + offset_name_2
  );

}
