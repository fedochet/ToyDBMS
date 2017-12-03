#include "pcrossproductnode.h"
#include "../../utils/utils.h"
#include "../../utils/bd_utils.h"

using namespace std;

PCrossProductNode::PCrossProductNode(PGetNextNode* left, PGetNextNode* right, LCrossProductNode* source)
    : PGetNextNode(source, left, right) {
}

query_result PCrossProductNode::GetNextBlock() {
  if (current_left_block.empty()) {
    UpdateLeftBlock();
  }

  if (current_right_block.empty()) {
    UpdateRightBlock();
  }

  query_result result_block;

  while (result_block.size() < BLOCK_SIZE
         && !current_right_block.empty()
         && !current_left_block.empty()) {

    if (current_right_pos >= current_right_block.size()) {
      if (UpdateRightBlock()) {
        current_left_pos++;
      }
    }

    if (current_left_pos >= current_left_block.size()) {
      UpdateLeftBlock();
      break;
    }

    for (; current_right_pos < current_right_block.size(); current_right_pos++) {
      auto &right_row = current_right_block[current_right_pos];
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

void PCrossProductNode::UpdateLeftBlock() {
  current_left_block = dynamic_cast<PGetNextNode*>(left)->GetNextBlock();
  current_left_pos = 0;
}

bool PCrossProductNode::UpdateRightBlock() {
  auto right_node = dynamic_cast<PGetNextNode*>(right);
  current_right_block = right_node->GetNextBlock();
  current_right_pos = 0;

  if (current_right_block.empty()) {
    right_node->Rewind();
    current_right_block = right_node->GetNextBlock();
    return true;
  }

  return false;
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

void PCrossProductNode::Rewind() {
  current_left_pos = 0;
  current_right_pos = 0;

  current_left_block.empty();
  current_right_block.empty();

  dynamic_cast<PGetNextNode*>(left)->Rewind();
  dynamic_cast<PGetNextNode*>(right)->Rewind();
}

PCrossProductNode::~PCrossProductNode() {
  delete left;
  delete right;
}
