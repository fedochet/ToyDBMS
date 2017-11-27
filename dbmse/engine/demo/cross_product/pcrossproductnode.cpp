#include "pcrossproductnode.h"
#include "../../utils/utils.h"

using namespace std;

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LCrossProductNode* source)
    : PGetNextNode(source, left, right) {
}

query_result PCrossProductNode::GetNextBlock() {

  if (right_node_table.empty()) {
    LoadRightBlock();
  }

  if (current_left_block.empty()) {
    UpdateLeftBlock();
  }

  query_result result_block;
  while (result_block.size() < BLOCK_SIZE
         && !right_node_table.empty()
         && !current_left_block.empty()) {

    if (current_right_pos >= right_node_table.size()) {
      current_right_pos = 0;
      current_left_pos++;
    }

    if (current_left_pos >= current_left_block.size()) {
      UpdateLeftBlock();
      break;
    }

    for (; current_right_pos < right_node_table.size(); current_right_pos++) {
      auto &right_row = right_node_table[current_right_pos];
      auto tmp_result = current_left_block[current_left_pos];
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

void PCrossProductNode::LoadRightBlock() {
  right_node_table = dynamic_cast<PGetNextNode*>(right)->GetNext();
  current_right_pos = 0;
}

void PCrossProductNode::UpdateLeftBlock() {
  current_left_block = dynamic_cast<PGetNextNode*>(left)->GetNextBlock();
  current_left_pos = 0;
}

void PCrossProductNode::Print(size_t indent) {
  for (int i = 0; i < indent; i++) {
    cout << " ";
  }
  cout << "CROSS-PRODUCT" << endl;
  left->Print(indent + 2);
  cout << endl;
  right->Print(indent + 2);
}

PCrossProductNode::~PCrossProductNode() {
  delete left;
  delete right;
}
