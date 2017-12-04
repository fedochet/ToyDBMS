#include "pcrossproductnode.h"
#include "../../utils/utils.h"
#include "../../utils/bd_utils.h"

using namespace std;

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LCrossProductNode* source)
    : PGetNextNode(source, left, right)
    , left_iterator(left) {
}

query_result PCrossProductNode::GetNextBlock() {

  if (current_right_block.empty()) {
    UpdateRightBlock();
  }

  query_result result_block;

  while (result_block.size() < BLOCK_SIZE
         && !current_right_block.empty()
         && !left_iterator.Closed()) {

    if (current_right_pos >= current_right_block.size()) {
      UpdateRightBlock();
    }

    if (left_iterator.Closed()) {
      continue;
    }

    for (; current_right_pos < current_right_block.size(); current_right_pos++) {
      auto &right_row = current_right_block[current_right_pos];
      auto tmp_result = *left_iterator;
      utils::append_to_back(tmp_result, right_row);
      result_block.push_back(tmp_result);

      if (result_block.size() >= BLOCK_SIZE) {
        current_right_pos++;
        break;
      }
    }
  }

  return result_block;
}

size_t PCrossProductNode::GetAttrNum() {
  return left->GetAttrNum() + right->GetAttrNum();
}

void PCrossProductNode::UpdateRightBlock() {
  auto right_node = dynamic_cast<PGetNextNode*>(right);
  current_right_block = right_node->GetNextBlock();
  current_right_pos = 0;

  if (current_right_block.empty()) {
    right_node->Rewind();
    current_right_block = right_node->GetNextBlock();
    ++left_iterator;
  }
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
  current_right_pos = 0;

  left_iterator.Rewind();
  dynamic_cast<PGetNextNode*>(left)->Rewind();
  dynamic_cast<PGetNextNode*>(right)->Rewind();
}

PCrossProductNode::~PCrossProductNode() {
  delete left;
  delete right;
}
