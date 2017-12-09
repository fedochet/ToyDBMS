#include "lcrossproductnode.h"
#include "../../utils/utils.h"

LCrossProductNode::LCrossProductNode(LAbstractNode* left, LAbstractNode* right)
    : LAbstractNode(left, right) {
  utils::append_to_back(fieldNames, left->fieldNames);
  utils::append_to_back(fieldNames, right->fieldNames);

  utils::append_to_back(fieldTypes, left->fieldTypes);
  utils::append_to_back(fieldTypes, right->fieldTypes);

  utils::append_to_back(fieldOrders, left->fieldOrders);
  fieldOrders.insert(std::end(fieldOrders), right->fieldOrders.size(), CS_UNKNOWN);
}

LCrossProductNode::~LCrossProductNode() {
  delete left;
  delete right;
}
