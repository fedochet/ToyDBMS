#include <tuple>
#include "lselectnode.h"

LSelectNode::LSelectNode(BaseTable &table,
                         std::vector<Predicate> predicates)
    : LAbstractNode(nullptr, nullptr), predicates(predicates), iteratorpos(0), table(table) {
  for (int i = 0; i < table.nbAttr; i++) {
    std::string tmp = table.relpath + "." + table.vnames[i];
    std::vector<std::string> tmp2;
    tmp2.push_back(tmp);
    fieldNames.push_back(tmp2);
  }
  fieldTypes = table.vtypes;
  fieldOrders = table.vorders;
}

BaseTable &LSelectNode::GetBaseTable() {
  return table;
}

std::tuple<int, Predicate> LSelectNode::GetNextPredicate() {
  if (predicates.size() == 0 || iteratorpos >= predicates.size()) {
    return std::make_tuple(1, Predicate());
  }
  return std::make_tuple(0, predicates[iteratorpos++]);
}

void LSelectNode::ResetIterator() {
  iteratorpos = 0;
}


LSelectNode::~LSelectNode() = default;
