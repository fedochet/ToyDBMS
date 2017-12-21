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
//      1) support for restricting physical joins node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for PredicateInfo and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include <algorithm>
#include <tuple>
#include "interface.h"
#include "../utils/utils.h"

LAbstractNode::LAbstractNode(LAbstractNode* left, LAbstractNode* right) : left(left), right(right) {}

LAbstractNode::~LAbstractNode() {
  delete left;
  delete right;
}

LAbstractNode* LAbstractNode::GetLeft() {
  return left;
}

LAbstractNode* LAbstractNode::GetRight() {
  return right;
}


LUniqueNode::LUniqueNode(LAbstractNode* child) : LAbstractNode(child, nullptr) {}

LUniqueNode::~LUniqueNode() = default;


/* Physical nodes*/

PResultNode::PResultNode(PResultNode* left, PResultNode* right, LAbstractNode* p)
    : prototype(p), left(left), right(right), record_position(0) {}

PResultNode::~PResultNode() = default;

std::tuple<ErrCode, std::vector<Value>> PResultNode::GetRecord() {
  std::vector<Value> vals;
  if (record_position == data.size()) return std::make_tuple(EC_FINISH, vals);
  for (size_t i = 0; i < GetAttrNum(); i++) {
    vals.push_back(data[record_position][i]);
  }
  record_position++;
  return std::make_tuple(EC_OK, vals);
}
