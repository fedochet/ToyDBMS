#include <tuple>
#include "lselectnode.h"

using namespace std;

LSelectNode::LSelectNode(BaseTable &table, vector<PredicateInfo> predicates)
    : LAbstractNode(nullptr, nullptr), predicates(predicates), iteratorpos(0), table(table) {
  for (size_t i = 0; i < table.nbAttr; i++) {
    string tmp = table.relpath + "." + table.vnames[i];
    vector<string> tmp2;
    tmp2.push_back(tmp);
    fieldNames.push_back(tmp2);
  }
  fieldTypes = table.vtypes;
  fieldOrders = table.vorders;
}

BaseTable &LSelectNode::GetBaseTable() {
  return table;
}

void LSelectNode::ResetIterator() {
  iteratorpos = 0;
}


LSelectNode::~LSelectNode() = default;
