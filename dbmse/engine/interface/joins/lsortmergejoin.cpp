#include "lsortmergejoin.h"

using namespace std;

LSortMergeJoin::LSortMergeJoin(LAbstractNode* left, LAbstractNode* right, string left_offset, string right_offset)
    : LAbstractNode(left, right), left_offset(left_offset), right_offset(right_offset) {}
