#include "ljoinnode.h"
#include "algorithm"
#include "../../utils/utils.h"

LJoinNode::LJoinNode(LAbstractNode* left, LAbstractNode* right,
                     std::string offset1, std::string offset2, int memorylimit) : LAbstractNode(left, right) {
  using namespace utils;
  
  this->offset1 = offset1;
  this->offset2 = offset2;
  this->memorylimit = memorylimit;

  // TODO: disgusting, fix this
  name_aliases match;
  ValueType vt;
  COLUMN_SORT cs;
  for (size_t i = 0; i < left->fieldNames.size(); i++) {
    for (size_t j = 0; j < right->fieldNames.size(); j++) {
      name_aliases l = left->fieldNames[i];
      name_aliases r = right->fieldNames[j];

      if (contains(l, offset1)) {
        if (contains(r, offset2)) {
          match = l;
          append_to_back(match, r);
          vt = left->fieldTypes[i];
          cs = left->fieldOrders[i];
        }
      } else if (contains(l, offset2)) {
        if (contains(r, offset1)) {
          match = l;
          append_to_back(match, r);
          vt = left->fieldTypes[i];
          cs = left->fieldOrders[i];
        }
      }
    }
  }

  for (size_t i = 0; i < left->fieldNames.size(); i++) {
    name_aliases l = left->fieldNames[i];
    if (!contains(l, offset1))
      if (!contains(l, offset2)) {
        fieldNames.push_back(l);
        fieldTypes.push_back(left->fieldTypes[i]);
        fieldOrders.push_back(left->fieldOrders[i]);
      }
  }

  for (size_t i = 0; i < right->fieldNames.size(); i++) {
    name_aliases r = right->fieldNames[i];
    if (!contains(r, offset1))
      if (!contains(r, offset2)) {
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
