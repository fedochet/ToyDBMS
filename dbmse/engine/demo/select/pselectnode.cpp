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
//      1) support for restricting physical join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include <iostream>
#include <fstream>

#include <vector>
#include <sstream>

#include "pselectnode.h"
#include "../../utils/utils.h"

using namespace std;

PSelectNode::PSelectNode(LAbstractNode* p, vector<Predicate> predicate)
    : PGetNextNode(p, nullptr, nullptr), table(dynamic_cast<LSelectNode*>(p)->GetBaseTable()), predicates(predicate),
      pos(0) {
  data.clear();
  Initialize();
}

template<typename T>
bool apply_comparison_predicate(const PredicateType &type, const T &original, const T &compare_to) {
  switch (type) {
    case PT_EQUALS:
      if (original == compare_to) {
        return true;
      }
      break;
    case PT_GREATERTHAN:
      if (original >= compare_to) {
        return true;
      }
      break;
  }

  return false;

}

bool apply_int_predicate(const Value &compared_attribute, const Predicate &predicate) {
  return apply_comparison_predicate(predicate.ptype, compared_attribute.vint, predicate.vint);
}

bool apply_string_predicate(const Value &compared_attribute, const Predicate &predicate) {
  return apply_comparison_predicate(predicate.ptype, compared_attribute.vstr, predicate.vstr);
}

bool
matches_predicates(const BaseTable &table, const vector<Value> &record, const vector<Predicate> &predicates) {
  for (auto &predicate: predicates) {
    if (table.vtypes[predicate.attribute] == predicate.vtype) {
      auto &compared_attribute = record[predicate.attribute];
      switch (predicate.vtype) {
        case VT_INT:
          if (!apply_int_predicate(compared_attribute, predicate)) {
            return false;
          }
          break;
        case VT_STRING:
          if (!apply_string_predicate(compared_attribute, predicate)) {
            return false;
          }
          break;
        default:
          throw runtime_error("Could not match predicate type!");
      }
    }
  }

  return true;
}

void PSelectNode::Initialize() {
  string line;
  ifstream table_file(table.relpath);
  if (table_file) {
    // skipping first 4 lines
    getline(table_file, line);
    getline(table_file, line);
    getline(table_file, line);
    getline(table_file, line);

    while (getline(table_file, line)) {
      vector<Value> tmp;
      string word;
      istringstream iss(line, istringstream::in);
      int field_index = 0;
      while (iss >> word) {
        // Yeah, no predicates :) -- Homework
        Value h;
        if (prototype->fieldTypes[field_index] == VT_INT) {
          h = Value(stoi(word));
        } else {
          h = Value(word);
        }
        tmp.push_back(h);
        field_index++;
      }

      if (matches_predicates(this->table, tmp, predicates)) {
        data.push_back(tmp);
      }
    }

    table_file.close();
  } else {
    cout << "Unable to open file";
  }
}

query_result PSelectNode::GetNext() {
  query_result result;

  auto next_block = GetNextBlock();
  while (!next_block.empty()) {
    utils::append_to_back(result, next_block);
    next_block = GetNextBlock();
  }

  return result;
}

query_result PSelectNode::GetNextBlock() {
  if (pos > data.size()) {
    return query_result();
  }

  auto block_start = data.begin() + pos;
  auto block_end = min(block_start + BLOCK_SIZE, data.end());
  pos += BLOCK_SIZE;

  return query_result(block_start, block_end);
}

void PSelectNode::Print(int indent) {
  for (int i = 0; i < indent; i++) {
    cout << " ";
  }

  cout << "SCAN " << table.relpath << " with predicates ";

  if (!predicates.empty()) {
    cout << predicates[0];
  } else {
    cout << "NULL" << endl;
  }

  if (left) left->Print(indent + 2);
  if (right) right->Print(indent + 2);
}

