#include <tuple>
#include "lselectnode.h"

using namespace std;

LSelectNode::LSelectNode(BaseTable &table, vector<Predicate> predicates)
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

tuple<int, Predicate> LSelectNode::GetNextPredicate() {
  if (predicates.empty() || iteratorpos >= predicates.size()) {
    return make_tuple(1, Predicate());
  }
  return make_tuple(0, predicates[iteratorpos++]);
}

void LSelectNode::ResetIterator() {
  iteratorpos = 0;
}


LSelectNode::~LSelectNode() = default;
