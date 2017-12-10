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
//      1) support for restricting physical joins node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include<algorithm>

#include "pnestedloopjoinnode.h"

using namespace std;

PNestedLoopJoinNode::PNestedLoopJoinNode(LNestedLoopJoinNode* p, PGetNextNode* left, PGetNextNode* right)
    : PGetNextNode(p, left, right), merger(p), left_iterator(left), right_iterator(right) {

    left_join_offset = p->GetLeftOffset().first;
    right_join_offset = p->GetRightOffset().first;
}

PNestedLoopJoinNode::~PNestedLoopJoinNode() {
    delete left;
    delete right;
}

size_t PNestedLoopJoinNode::GetAttrNum() {
    return left->GetAttrNum() + right->GetAttrNum() - 1;
}

query_result PNestedLoopJoinNode::GetNextBlock() {
    PGetNextNode* l = dynamic_cast<PGetNextNode*>(left);
    PGetNextNode* r = dynamic_cast<PGetNextNode*>(right);
    LAbstractNode* lp = l->prototype;
    LAbstractNode* rp = r->prototype;
    vector<name_aliases> ln = lp->fieldNames;
    vector<name_aliases> rn = rp->fieldNames;

    ValueType vt = lp->fieldTypes[left_join_offset];

    query_result result_block;
    while (result_block.size() < BLOCK_SIZE && !left_iterator.Closed()) {

        if (right_iterator.Closed()) {
            right_iterator.Rewind();
            ++left_iterator;
        }

        if (left_iterator.Closed()) {
            break;
        }

        while (!right_iterator.Closed()) {
            auto &left_row = *left_iterator;
            auto &right_row = *right_iterator;

            if (left_row[left_join_offset].vtype != vt || left_row[left_join_offset] != right_row[right_join_offset]) {
                ++right_iterator;
                continue;
            }

            result_block.push_back(merger.MergeRows(left_row, right_row));

            ++right_iterator;
            if (result_block.size() >= BLOCK_SIZE) {
                break;
            }
        }
    }

    return result_block;
}

void PNestedLoopJoinNode::Rewind() {
    left_iterator.Rewind();
    right_iterator.Rewind();
}

void PNestedLoopJoinNode::Print(size_t indent) {
    for (size_t i = 0; i < indent; i++) {
        cout << " ";
    }
    cout << "NL-JOIN: "
         << (dynamic_cast<LNestedLoopJoinNode*>(prototype)->GetLeftOffset().second)
         << "="
         << (dynamic_cast<LNestedLoopJoinNode*>(prototype)->GetRightOffset().second)
         << endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}
