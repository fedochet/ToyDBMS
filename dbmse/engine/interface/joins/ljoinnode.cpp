#include "ljoinnode.h"
#include "algorithm"

LJoinNode::LJoinNode(LAbstractNode* left, LAbstractNode* right,
                     std::string offset1, std::string offset2, int memorylimit) : LAbstractNode(left, right) {
  this->offset1 = offset1;
  this->offset2 = offset2;
  this->memorylimit = memorylimit;

  // TODO: disgusting, fix this
  std::vector<std::string> match;
  ValueType vt;
  COLUMN_SORT cs;
  for (size_t i = 0; i < left->fieldNames.size(); i++) {
    for (size_t j = 0; j < right->fieldNames.size(); j++) {
      std::vector<std::string> l = left->fieldNames[i];
      std::vector<std::string> r = right->fieldNames[j];

      if (std::find(l.begin(), l.end(), offset1) != l.end()) {
        if (std::find(r.begin(), r.end(), offset2) != r.end()) {
          match = l;
          match.insert(std::end(match), std::begin(r), std::end(r));
          vt = left->fieldTypes[i];
          cs = left->fieldOrders[i];
        }
      } else if (std::find(l.begin(), l.end(), offset2) != l.end()) {
        if (std::find(r.begin(), r.end(), offset1) != r.end()) {
          match = l;
          match.insert(std::end(match), std::begin(r), std::end(r));
          vt = left->fieldTypes[i];
          cs = left->fieldOrders[i];
        }
      }
    }
  }

  for (size_t i = 0; i < left->fieldNames.size(); i++) {
    std::vector<std::string> l = left->fieldNames[i];
    if (std::find(l.begin(), l.end(), offset1) == l.end())
      if (std::find(l.begin(), l.end(), offset2) == l.end()) {
        fieldNames.push_back(l);
        fieldTypes.push_back(left->fieldTypes[i]);
        fieldOrders.push_back(left->fieldOrders[i]);
      }
  }

  for (size_t i = 0; i < right->fieldNames.size(); i++) {
    std::vector<std::string> r = right->fieldNames[i];
    if (std::find(r.begin(), r.end(), offset1) == r.end())
      if (std::find(r.begin(), r.end(), offset2) == r.end()) {
        fieldNames.push_back(r);
        fieldTypes.push_back(right->fieldTypes[i]);
        fieldOrders.push_back(right->fieldOrders[i]);

      }

  }
  fieldNames.push_back(match);
  fieldTypes.push_back(vt);
  fieldOrders.push_back(cs);

}

LJoinNode::~LJoinNode() {
  delete left;
  delete rigth;
}
