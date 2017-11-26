#include "pprojectnode.h"
#include "../../utils/utils.h"

using namespace std;

PProjectNode::PProjectNode(PGetNextNode* from, LProjectNode* lProjectNode)
    : PGetNextNode(lProjectNode, from, nullptr) {
}

query_result PProjectNode::GetNextBlock() {
  auto current_block = dynamic_cast<PGetNextNode*>(left)->GetNextBlock();
  query_result result_block;

  for (auto &record: current_block) {
    vector<Value> projected_record;
    for (auto &field_index: dynamic_cast<LProjectNode*>(prototype)->offsets) {
      projected_record.push_back(record[field_index]);
    }
    result_block.push_back(projected_record);
  }

  return result_block;
}

void PProjectNode::Print(int indent) {
  for (int i = 0; i < indent; i++){
    std::cout << " ";
  }
  std::cout << "Project Node, keeped names: ";
  for (auto& name_to_keep: dynamic_cast<LProjectNode*>(prototype)->fieldNames) {
    std::cout << name_to_keep[0] << " ";
  }
  cout << endl;
  left->Print(indent + 2);
  cout << endl;
}
