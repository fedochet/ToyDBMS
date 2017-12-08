#include "bd_utils.h"

using namespace utils;

BlockIterator::BlockIterator(PGetNextNode* node)
    : node(node), current_pos(0), current_block(node->GetNextBlock()) {
}

const query_result_row &utils::BlockIterator::operator*() const {
  if (Closed()) {
    throw std::runtime_error("No more data in table, Iterator is closed!");
  }

  return current_block[current_pos];
}

BlockIterator &utils::BlockIterator::operator++() {
  current_pos++;

  if (current_pos >= current_block.size()) {
    current_pos = 0;
    current_block = node->GetNextBlock();
  }

  return *this;
}

void utils::BlockIterator::Rewind() {
  node->Rewind();
  current_block.clear();
  current_pos = 0;
  ++(*this);
}

bool BlockIterator::Closed() const {
  return current_block.empty();
}

AdvanceBlockIterator::AdvanceBlockIterator(PGetNextNode* node, size_t offset)
    : iterator(node), offset(offset) {
  ++(*this);
}

const query_result &AdvanceBlockIterator::operator*() const {
  return current_block;
}

AdvanceBlockIterator &AdvanceBlockIterator::operator++() {
  current_block.clear();

  current_block.push_back(*iterator);
  while (!(++iterator).Closed()) {
    if ((*iterator)[offset] == current_block.back()[offset]) {
      current_block.push_back(*iterator);
    } else {
      break;
    }
  }
  return *this;
}

void AdvanceBlockIterator::Rewind() {
  iterator.Rewind();
  current_block.empty();
}

bool AdvanceBlockIterator::Closed() const {
  return current_block.empty();
}
