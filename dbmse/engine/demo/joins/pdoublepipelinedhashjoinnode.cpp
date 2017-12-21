#include "pdoublepipelinedhashjoinnode.h"

using namespace std;

PDoublePipelinedHashJoinNode::PDoublePipelinedHashJoinNode(LDoublePipelinedHashJoin* p, PGetNextNode* left,
                                                           PGetNextNode* right)
    : PGetNextNode(p, left, right), left_join_offset(p->GetLeftOffset()),
      right_join_offset(p->GetRightOffset()),
      merger(p),
      left_iterator(left),
      right_iterator(right) {}


void PDoublePipelinedHashJoinNode::Print(size_t indent) {
    for (size_t i = 0; i < indent; i++) {
        cout << " ";
    }
    cout << "DOUBLE-PIPLINED-HASH-JOIN: "
         << left_join_offset.second
         << "="
         << right_join_offset.second
         << endl;
    left->Print(indent + 2);
    right->Print(indent + 2);
}

query_result PDoublePipelinedHashJoinNode::GetNextBlock() {
    query_result result;

    while (result.size() < BLOCK_SIZE && (!left_iterator.Closed() && !right_iterator.Closed())) {
        auto& current_iterator = from_left ? left_iterator : right_iterator;
        auto& current_row = *current_iterator;
        auto& current_table = from_left ? left_hash_table : right_hash_table;
        auto& opposite_table = from_left ? right_hash_table : left_hash_table;
        auto& current_row_offset = from_left ? left_join_offset : right_join_offset;

        auto& current_block = opposite_table[current_row[current_row_offset.first]];

        if (hash_table_block_pos >= current_block.size()) {
            hash_table_block_pos = 0;
            current_table[current_row[current_row_offset.first]].push_back(current_row);
            ++current_iterator;
            from_left = !from_left;
            continue;
        }

        while (result.size() < BLOCK_SIZE && hash_table_block_pos < current_block.size()) {
            if (from_left) {
                result.push_back(merger.MergeRows(current_row, current_block[hash_table_block_pos]));
            } else {
                result.push_back(merger.MergeRows(current_block[hash_table_block_pos], current_row));
            }
            hash_table_block_pos++;
        }
    }

    while (result.size() < BLOCK_SIZE && !left_iterator.Closed()) {
        auto& current_row = *left_iterator;
        auto& current_block = right_hash_table[current_row[left_join_offset.first]];

        if (hash_table_block_pos >= current_block.size()) {
            ++left_iterator;
            hash_table_block_pos = 0;
            continue;
        }

        while (result.size() < BLOCK_SIZE && hash_table_block_pos < current_block.size()) {
            result.push_back(merger.MergeRows(current_row, current_block[hash_table_block_pos]));
            hash_table_block_pos++;
        }
    }

    while (result.size() < BLOCK_SIZE && !right_iterator.Closed()) {
        auto& current_row = *right_iterator;
        auto& current_block = left_hash_table[current_row[right_join_offset.first]];

        if (hash_table_block_pos >= current_block.size()) {
            ++right_iterator;
            hash_table_block_pos = 0;
            continue;
        }

        while (result.size() < BLOCK_SIZE && hash_table_block_pos < current_block.size()) {
            result.push_back(merger.MergeRows(current_block[hash_table_block_pos], current_row));
            hash_table_block_pos++;
        }
    }

    return result;
}

void PDoublePipelinedHashJoinNode::Rewind() {
    left_iterator.Rewind();
    right_iterator.Rewind();
}

size_t PDoublePipelinedHashJoinNode::GetAttrNum() {
    return PGetNextNode::GetAttrNum();
}

PDoublePipelinedHashJoinNode::~PDoublePipelinedHashJoinNode() {
    delete left;
    delete right;
}