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
//      1) support for restricting physical join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include<cstddef>
#include<algorithm>

#include "pjoinnode.h"
#include "../../utils/utils.h"
#include "../../utils/bd_utils.h"

using namespace std;

typedef vector<string> name_aliases;

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right,
                     LAbstractNode* p) : PGetNextNode(p, left, right) {
  Initialize();
}

PJoinNode::~PJoinNode() {
  delete left;
  delete right;
}

void PJoinNode::Initialize() {
  PGetNextNode* l = (PGetNextNode*) left;
  PGetNextNode* r = (PGetNextNode*) right;
  LAbstractNode* lp = l->prototype;
  LAbstractNode* rp = r->prototype;
  vector<name_aliases> ln = lp->fieldNames;
  vector<name_aliases> rn = rp->fieldNames;

  left_join_offset = FindColumnOffset(ln);
  right_join_offset = FindColumnOffset(rn);
}

size_t PJoinNode::GetAttrNum() {
  return left->GetAttrNum() + right->GetAttrNum() - 1;
}

query_result PJoinNode::GetNextBlock() {
  PGetNextNode* l = (PGetNextNode*) left;
  PGetNextNode* r = (PGetNextNode*) right;
  LAbstractNode* lp = l->prototype;
  LAbstractNode* rp = r->prototype;
  vector<name_aliases> ln = lp->fieldNames;
  vector<name_aliases> rn = rp->fieldNames;

  ValueType vt = lp->fieldTypes[left_join_offset];

  if (right_node_table.empty()) {
    LoadRightBlock();
  }

  if (current_left_block.empty()) {
    UpdateLeftBlock();
  }

  return utils::GetNextBlock(
      current_left_pos, current_left_block,
      current_right_pos, right_node_table,
      [&] { UpdateLeftBlock(); },
      [&](query_result &result, query_result_row &left_row, query_result_row &right_row) {
          if (left_row[left_join_offset].vtype != vt || left_row[left_join_offset] != right_row[right_join_offset]) {
            return;
          }

          query_result_row tmp;
          for (int k = 0; k < ln.size(); k++) {
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
          result.push_back(tmp);
      }
  );
}

void PJoinNode::LoadRightBlock() {
  right_node_table = dynamic_cast<PGetNextNode*>(right)->GetAllData();
  current_right_pos = 0;
}

void PJoinNode::UpdateLeftBlock() {
  current_left_block = dynamic_cast<PGetNextNode*>(left)->GetNextBlock();
  current_left_pos = 0;
}

size_t PJoinNode::FindColumnOffset(const vector<name_aliases> &names) const {
  auto offset_name_1 = ((LJoinNode*) prototype)->offset1;
  auto offset_name_2 = ((LJoinNode*) prototype)->offset2;

  for (size_t i = 0; i < names.size(); i++) {
    ptrdiff_t lpos1 = utils::find(names[i], offset_name_1);
    ptrdiff_t lpos2 = utils::find(names[i], offset_name_2);

    if (lpos1 < names[i].size() || lpos2 < names[i].size()) {
      return i;
    }
  }

  throw runtime_error(
      string("Cannot join by column named ") + offset_name_1 + " or " + offset_name_2
  );
}

void PJoinNode::Print(size_t indent) {
  for (int i = 0; i < indent; i++) {
    cout << " ";
  }
  cout << "NL-JOIN: " << ((LJoinNode*) prototype)->offset1 << "=" << ((LJoinNode*) prototype)->offset2 << endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}
