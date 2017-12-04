#include "pcrossproductnode.h"

using namespace std;

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LCrossProductNode* source)
    : PGetNextNode(source, left, right)
    , left_iterator(left)
    , right_iterator(right) {
}

query_result PCrossProductNode::GetNextBlock() {

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
      auto &right_row = *right_iterator;
      auto tmp_result = *left_iterator;
      utils::append_to_back(tmp_result, right_row);
      result_block.push_back(tmp_result);
      ++right_iterator;
    }
  }

  return result_block;
}

size_t PCrossProductNode::GetAttrNum() {
  return left->GetAttrNum() + right->GetAttrNum();
}

void PCrossProductNode::Print(size_t indent) {
  for (size_t i = 0; i < indent; i++) {
    cout << " ";
  }
  cout << "CROSS-PRODUCT" << endl;
  left->Print(indent + 2);
  cout << endl;
  right->Print(indent + 2);
}

void PCrossProductNode::Rewind() {
  left_iterator.Rewind();
  right_iterator.Rewind();
}

PCrossProductNode::~PCrossProductNode() {
  delete left;
  delete right;
}
