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

using namespace std;

typedef vector<string> name_aliases;

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right,
                     LAbstractNode* p) : PGetNextNode(p, left, right) {
  pos = 0;
  Initialize();
}

PJoinNode::~PJoinNode() {
  delete left;
  delete right;
}

query_result PJoinNode::GetNext() {
  return data;
}

void PJoinNode::Initialize() {
  PGetNextNode* l = (PGetNextNode*) left;
  PGetNextNode* r = (PGetNextNode*) right;
  LAbstractNode* lp = l->prototype;
  LAbstractNode* rp = r->prototype;
  vector<name_aliases> ln = lp->fieldNames;
  vector<name_aliases> rn = rp->fieldNames;

  query_result lres = l->GetNext();
  query_result rres = r->GetNext();

  size_t lpos = FindColumnOffset(ln);
  size_t rpos = FindColumnOffset(rn);

  ValueType vt = lp->fieldTypes[lpos];

  for (size_t i = 0; i < lres.size(); i++) {
    for (size_t j = 0; j < rres.size(); j++) {
      if (lres[i][lpos] != rres[j][rpos]) {
        continue;
      }

      vector<Value> tmp;
      for (int k = 0; k < ln.size(); k++) {
        if (k != lpos) {
          tmp.push_back(lres[i][k]);
        }
      }

      for (size_t k = 0; k < rn.size(); k++) {
        if (k != rpos) {
          tmp.push_back(rres[j][k]);
        }
      }

      tmp.push_back(lres[i][lpos]);
      data.push_back(tmp);
    }
  }
}

size_t PJoinNode::FindColumnOffset(const vector<name_aliases> &names) const {
  auto offset1 = ((LJoinNode*) prototype)->offset1;
  auto offset2 = ((LJoinNode*) prototype)->offset2;

  for (size_t i = 0; i < names.size(); i++) {
    ptrdiff_t lpos1 = find(names[i].begin(), names[i].end(), offset1) - names[i].begin();
    ptrdiff_t lpos2 = find(names[i].begin(), names[i].end(), offset2) - names[i].begin();

    if (lpos1 < names[i].size() || lpos2 < names[i].size()) {
      return i;
    }
  }

  throw runtime_error(
      string("Cannot join by column named ") + offset1 + " or " + offset2
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
