#include "lsortmergejoin.h"
#include "../../utils/utils.h"

using namespace std;

LSortMergeJoinNode::LSortMergeJoinNode(LAbstractNode* left, LAbstractNode* right, string left_offset, string right_offset)
    : LAbstractNode(left, right), left_offset(left_offset), right_offset(right_offset) {
  using namespace utils;

  // TODO: disgusting, fix this
  name_aliases match;
  ValueType vt;
  COLUMN_SORT cs;
  for (size_t i = 0; i < left->fieldNames.size(); i++) {
    for (size_t j = 0; j < right->fieldNames.size(); j++) {
      name_aliases l = left->fieldNames[i];
      name_aliases r = right->fieldNames[j];

      if (contains(l, left_offset) && contains(r, right_offset)) {
        AssertColumnsSortedSame(i, j);
        match = l;
        append_to_back(match, r);
        vt = left->fieldTypes[i];
        cs = left->fieldOrders[i];
      } else if (contains(l, right_offset) && contains(r, left_offset)) {
        AssertColumnsSortedSame(i, j);
        match = l;
        append_to_back(match, r);
        vt = left->fieldTypes[i];
        cs = left->fieldOrders[i];
      }
    }
  }

  for (size_t i = 0; i < left->fieldNames.size(); i++) {
    name_aliases l = left->fieldNames[i];
    if (!contains(l, left_offset))
      if (!contains(l, right_offset)) {
        fieldNames.push_back(l);
        fieldTypes.push_back(left->fieldTypes[i]);
        fieldOrders.push_back(left->fieldOrders[i]);
      }
  }

  for (size_t i = 0; i < right->fieldNames.size(); i++) {
    name_aliases r = right->fieldNames[i];
    if (!contains(r, left_offset))
      if (!contains(r, right_offset)) {
        fieldNames.push_back(r);
        fieldTypes.push_back(right->fieldTypes[i]);
        fieldOrders.push_back(right->fieldOrders[i]);
      }
  }

  fieldNames.push_back(match);
  fieldTypes.push_back(vt);
  fieldOrders.push_back(cs);
}

LSortMergeJoinNode::~LSortMergeJoinNode() {
  delete left;
  delete rigth;
}

void LSortMergeJoinNode::AssertColumnsSortedSame(size_t i, size_t j) {
//  bool left_field_sorted = GetLeft()->fieldOrders[i] == CS_DESCENDING || GetRight()->fieldOrders[i] == CS_ASCENDING;
  bool left_field_sorted = GetLeft()->fieldOrders[i] == CS_ASCENDING;
  if (!left_field_sorted || GetLeft()->fieldOrders[i] != GetRight()->fieldOrders[j]) {
    throw runtime_error("You cannot use sort-merge join to join not sorted fiedls!");
  }

}
