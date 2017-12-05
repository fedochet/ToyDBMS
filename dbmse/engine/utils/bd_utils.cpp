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

CachedBlockIterator::CachedBlockIterator(PGetNextNode* node) : BlockIterator(node) {
}

CachedBlockIterator &CachedBlockIterator::operator++() {
  if (rewinded) {
    current_cache_pos++;
    if (current_cache_pos >= cache.size()) {
      rewinded = false;
      current_cache_pos = 0;
      return *this;
    } else {
      return *this;
    }
  }

  if (!Closed()) {
    cache.push_back(**this);
  }

  BlockIterator::operator++();
  return *this;
}

CachedBlockIterator &CachedBlockIterator::RepeatCache() {
  if (!cache.empty()) {
    rewinded = true;
    current_cache_pos = 0;
  }
  return *this;
}

void CachedBlockIterator::ClearCache() {
  current_cache_pos = 0;
  cache.clear();
}

void CachedBlockIterator::Rewind() {
  BlockIterator::Rewind();
  ClearCache();
}

bool CachedBlockIterator::Closed() const {
  return !rewinded && BlockIterator::Closed();
}

const query_result_row &CachedBlockIterator::operator*() const {
  if (rewinded) {
    return cache[current_cache_pos];
  }

  return BlockIterator::operator*();
}
