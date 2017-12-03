// Toy DBMS, v0.4
// George Chernishev (c) 2016-2017, chernishev<at>google mail
// A task framework for undergraduate students at Saint-Petersburg Academic University, DBMS development course
// More details regarding the course can be found here: www.math.spbu.ru/user/chernishev/
// CHANGELOG:
// 0.4: no chance for efficiency competition, so, this year I reoriented task towards ideas:
//      1) tried to remove C code, now we are using lots of std:: stuff
//      2) implemented support for multiple attributes in the DBMS
//      3) code clean-up and restructurization
// 0.3: added:
//      1) support for restricting physical join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include <tuple>

#include "pgetnextnode.h"
#include "../utils/utils.h"

PGetNextNode::PGetNextNode(LAbstractNode* source, PResultNode* left, PResultNode* right)
    : PResultNode(left, right, source) {
  Initialize();
}

void PGetNextNode::Initialize() {
  return;
}

query_result PGetNextNode::GetAllData() {
  query_result result;

  auto next_block = GetNextBlock();
  while (!next_block.empty()) {
    utils::append_to_back(result, next_block);
    next_block = GetNextBlock();
  }

  return result;
}

size_t PGetNextNode::GetAttrNum() {
  return prototype->fieldNames.size();
}

query_result PGetNextNode::GetNextBlock() {
  if (current_position > data.size()) {
    return query_result();
  }

  auto block_start = data.begin() + current_position;
  auto block_end = min(block_start + BLOCK_SIZE, data.end());
  current_position += block_end - block_start;

  return query_result(block_start, block_end);
}

void PGetNextNode::Rewind() {
}

std::tuple<ErrCode, std::vector<Value>> PGetNextNode::GetRecord() {
  if (data.empty()) {
    data = GetAllData();
  }

  return PResultNode::GetRecord();
}
