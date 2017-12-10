#include "phashjoinnode.h"

using namespace std;

PHashJoinNode::PHashJoinNode(LAbstractJoinNode* p, PGetNextNode* left, PGetNextNode* right)
    : PGetNextNode(p, left, right),
      left_join_offset(p->GetLeftOffset()),
      right_join_offset(p->GetRightOffset()),
      merger(p),
      left_iterator(left),
      right_iterator(right) {}

void PHashJoinNode::Print(size_t indent) {
    for (size_t i = 0; i < indent; i++) {
        cout << " ";
    }
    cout << "HASH-JOIN: "
         << left_join_offset.second
         << "="
         << right_join_offset.second
         << endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}

query_result PHashJoinNode::GetNextBlock() {
    if (hash_table.empty()) {
        BuildHashTable();
    }

    query_result block;
    while (block.size() < BLOCK_SIZE && !left_iterator.Closed()) {
        auto& left_row = *left_iterator;
        auto& right_block = hash_table[left_row[left_join_offset.first]];

        if (hash_table_block_pos >= right_block.size()) {
            hash_table_block_pos = 0;
            ++left_iterator;
            continue;
        }

        while (block.size() < BLOCK_SIZE && hash_table_block_pos < right_block.size()) {
            block.push_back(merger.MergeRows(left_row, right_block[hash_table_block_pos]));
            hash_table_block_pos++;
        }
    }

    return block;
}

void PHashJoinNode::Rewind() {
    left_iterator.Rewind();
}

size_t PHashJoinNode::GetAttrNum() {
    return PGetNextNode::GetAttrNum();
}

PHashJoinNode::~PHashJoinNode() {
    delete left;
    delete right;
}

void PHashJoinNode::BuildHashTable() {
    if (right_iterator.Closed()) {
        hash_table.clear();
        right_iterator.Rewind();
    }

    while (!right_iterator.Closed()) {
        hash_table[(*right_iterator)[right_join_offset.first]].push_back(*right_iterator);
        ++right_iterator;
    }
}
