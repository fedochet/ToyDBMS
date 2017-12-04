#include "bd_utils.h"

using namespace utils;

BlockIterator::BlockIterator(PGetNextNode* node)
    : node(node)
    , current_pos(0)
    , current_block(node->GetNextBlock()) {
}

const query_result_row* utils::BlockIterator::operator* () const {
  if (Closed()) {
    throw std::runtime_error("No more data in table, Iterator is closed!");
  }

  return &current_block[current_pos];
}

const query_result_row &utils::BlockIterator::operator->() const {
  return *(this->operator*());
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
}

bool BlockIterator::Closed() const{
  return current_block.empty();
}
