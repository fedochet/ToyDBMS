#include "psortmergejoinnode.h"
#include "../../interface/joins/lsortmergejoin.h"

using namespace std;

PSortMergeJoinNode::PSortMergeJoinNode(LAbstractNode* p, PGetNextNode* left, PGetNextNode* right)
    : PGetNextNode(p, left, right)
    , left_iterator(left)
    , right_iterator(right) {
  vector<name_aliases> ln = left->prototype->fieldNames;
  vector<name_aliases> rn = right->prototype->fieldNames;
  left_join_offset = FindColumnOffset(ln);
  right_join_offset = FindColumnOffset(rn);

}

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

  ValueType vt = lp->fieldTypes[left_join_offset];

  query_result result_block;
  while (result_block.size() < BLOCK_SIZE && !left_iterator.Closed()) {

    if (right_iterator.Closed()) {
      right_iterator.Rewind();
      ++left_iterator;
    }

    if (left_iterator.Closed()) {
      break;
    }

    while (!right_iterator.Closed()) {
      auto &left_row = *left_iterator;
      auto &right_row = *right_iterator;

      if (left_row[left_join_offset].vtype != vt || left_row[left_join_offset] != right_row[right_join_offset]) {
        ++right_iterator;
        continue;
      }

      query_result_row tmp;
      for (size_t k = 0; k < ln.size(); k++) {
        if (k != left_join_offset) {
          tmp.push_back(left_row[k]);
        }
      }

      for (size_t k = 0; k < rn.size(); k++) {
        if (k != right_join_offset) {
          tmp.push_back(right_row[k]);
        }
      }

      tmp.push_back(left_row[left_join_offset]);
      result_block.push_back(tmp);
      ++right_iterator;
      if (result_block.size() >= BLOCK_SIZE) {
        break;
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
