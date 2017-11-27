#include "pcrossproductnode.h"
#include "../../utils/utils.h"
#include "../../utils/bd_utils.h"

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

  query_result result_block = utils::GetNextBlock(
      current_left_pos, current_left_block,
      current_right_pos, right_node_table,
      [&] { UpdateLeftBlock(); },
      [&](query_result &result, query_result_row left_row, query_result_row &right_row) {
          utils::append_to_back(left_row, right_row);
          result.push_back(left_row);
      });

  return result_block;
}

size_t PCrossProductNode::GetAttrNum() {
  return left->GetAttrNum() + right->GetAttrNum();
}

void PCrossProductNode::LoadRightBlock() {
  right_node_table = dynamic_cast<PGetNextNode*>(right)->GetAllData();
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
