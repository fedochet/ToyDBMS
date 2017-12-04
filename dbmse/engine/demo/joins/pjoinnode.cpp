// Toy DBMS, v0.4
// George Chernishev (c) 2016-2017, chernishev<at>google mail
// A task framework for undergraduate students at Saint-Petersburg Academic University, DBMS development course
// More details regarding the course can be found here: www.math.spbu.ru/user/chernishev/
// CHANGELOG:
// 0.4: no chance for efficiency competition, so, this year I reoriented task towards ideas:
//      1) tried to remove C code, now we are using lots of  stuff
//      2) implemented support for multiple attributes in the DBMS
//      3) code clean-up and restructurization
// 0.3: added:
//      1) support for restricting physical joins node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include<algorithm>

#include "pjoinnode.h"
#include "../../interface/joins/ljoinnode.h"

using namespace std;

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right,
                     LAbstractNode* p)
    : PGetNextNode(p, left, right)
    , left_iterator(left)
    , right_iterator(right) {

  vector<name_aliases> ln = left->prototype->fieldNames;
  vector<name_aliases> rn = right->prototype->fieldNames;
  left_join_offset = FindColumnOffset(ln);
  right_join_offset = FindColumnOffset(rn);
}

PJoinNode::~PJoinNode() {
  delete left;
  delete right;
}

size_t PJoinNode::GetAttrNum() {
  return left->GetAttrNum() + right->GetAttrNum() - 1;
}

query_result PJoinNode::GetNextBlock() {
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

void PJoinNode::Rewind() {
  left_iterator.Rewind();
  right_iterator.Rewind();
}

size_t PJoinNode::FindColumnOffset(const vector<name_aliases> &names) const {
  auto offset_name_1 = (dynamic_cast<LJoinNode*>(prototype))->offset1;
  auto offset_name_2 = (dynamic_cast<LJoinNode*>(prototype))->offset2;

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

void PJoinNode::Print(size_t indent) {
  for (size_t i = 0; i < indent; i++) {
    cout << " ";
  }
  cout << "NL-JOIN: "
       << (dynamic_cast<LJoinNode*>(prototype))->offset1
       << "="
       << (dynamic_cast<LJoinNode*>(prototype))->offset2
       << endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}
