#include "lsortmergejoin.h"
#include "../../utils/utils.h"
#include "labstractjoinnode.h"

using namespace std;

LSortMergeJoinNode::LSortMergeJoinNode(LAbstractNode* left, LAbstractNode* right, string left_offset, string right_offset)
    : LAbstractJoinNode(left, right, left_offset, right_offset) {
  using namespace utils;
  AssertColumnsSortedSame(GetLeftOffset().first, GetRightOffset().first);
}

void LSortMergeJoinNode::AssertColumnsSortedSame(size_t i, size_t j) {
//  bool left_field_sorted = GetLeft()->fieldOrders[i] == CS_DESCENDING || GetRight()->fieldOrders[i] == CS_ASCENDING;
  bool left_field_sorted = GetLeft()->fieldOrders[i] == CS_ASCENDING;
  if (!left_field_sorted || GetLeft()->fieldOrders[i] != GetRight()->fieldOrders[j]) {
    throw runtime_error("You cannot use sort-merge join to join not sorted fiedls!");
  }
}
