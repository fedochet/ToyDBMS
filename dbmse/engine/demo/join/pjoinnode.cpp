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

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right,
                     LAbstractNode* p): PGetNextNode(p, left, right) {
  pos = 0;
  Initialize();
}

PJoinNode::~PJoinNode(){
  delete left;
  delete right;
}

query_result PJoinNode::GetNext(){
  return data;
}

void PJoinNode::Initialize(){
  PGetNextNode* l = (PGetNextNode*)left;
  PGetNextNode* r = (PGetNextNode*)right;
  LAbstractNode* lp = l->prototype;
  LAbstractNode* rp = r->prototype;
  vector<vector<string>> ln = lp->fieldNames;
  vector<vector<string>> rn = rp->fieldNames;

  query_result lres = l->GetNext();
  query_result rres = r->GetNext();
  LAbstractNode* p = prototype;
  ptrdiff_t lpos, rpos;

  for (int i = 0; i < lp->fieldNames.size(); i++){
    ptrdiff_t lpos1 = find(ln[i].begin(), ln[i].end(), ((LJoinNode*)prototype)->offset1) - ln[i].begin();
    ptrdiff_t lpos2 = find(ln[i].begin(), ln[i].end(), ((LJoinNode*)prototype)->offset2) - ln[i].begin();

    if(lpos1 <= ln.size() || lpos1 <= ln.size()){
      if (lpos1 < lpos2)
        lpos = lpos1;
      else
        lpos = lpos2;
      break;
    }
  }

  for (int i = 0; i < rp->fieldNames.size(); i++){
    ptrdiff_t rpos1 = find(rn[i].begin(), rn[i].end(), ((LJoinNode*)prototype)->offset1) - rn[i].begin();
    ptrdiff_t rpos2 = find(rn[i].begin(), rn[i].end(), ((LJoinNode*)prototype)->offset2) - rn[i].begin();

    if(rpos1 <= rn.size() || rpos1 <= rn.size()){
      if (rpos1 < rpos2)
        rpos = rpos1;
      else
        rpos = rpos2;
      break;
    }
  }

  ValueType vt = lp->fieldTypes[lpos];

  for (int i = 0; i < lres.size(); i++)
    for (int j = 0; j < rres.size(); j++){
      bool join = false;
      if(vt == VT_INT){
        if((int)lres[i][lpos] == (int)rres[j][rpos]) join = true;
      }else{
        if((string)lres[i][lpos] == (string)rres[j][rpos]) join = true;
      }

      if (join != true) continue;

      vector<Value> tmp;
      for (int k = 0; k < ln.size(); k++){
        if(k != lpos){
            tmp.push_back(lres[i][k]);
        }
      }

      for (int k = 0; k < rn.size(); k++){
        if(k != rpos){
            tmp.push_back(rres[j][k]);
        }
      }

      tmp.push_back(lres[i][lpos]);


      data.push_back(tmp);
    }

}

void PJoinNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    cout << " ";
  }
  cout << "NL-JOIN: " << ((LJoinNode*)prototype)->offset1 <<"=" << ((LJoinNode*)prototype)->offset2 << endl;
  left->Print(indent + 2);
  right->Print(indent + 2);
}
