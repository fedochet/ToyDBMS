// Toy DBMS, v0.4
// George Chernishev (c) 2016-2017, chernishev<at>google mail
// A task framework for undergraduate students at Saint-Petersburg Academic University, DBMS development course
// More details regarding the course can be found here: www.math.spbu.ru/user/chernishev/
// CHANGELOG:
// 0.4: no chance for efficiency competition, so, this year I reoriented task towards ideas:
//      1) tried to remove C code, now we are using lots of std:: stuff
//      2) implemented support for multiple attributes in the DBMS
//      3) code clean-up and restructurization
// 0.3: added:
//      1) support for restricting physical joins node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for PredicateInfo and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "basics.h"

/* Logical nodes (query) */

class LAbstractNode{
  public:
    LAbstractNode(LAbstractNode* left, LAbstractNode* right);
    virtual ~LAbstractNode();
    LAbstractNode* GetLeft();
    LAbstractNode* GetRight();
    // schema-related info
    std::vector<std::vector<std::string>> fieldNames;
    std::vector<ValueType> fieldTypes;
    std::vector<COLUMN_SORT> fieldOrders;
  protected:
    LAbstractNode* left;
    LAbstractNode* right;
};

class LUniqueNode : public LAbstractNode{
  public:
    LUniqueNode(LAbstractNode* child);
    ~LUniqueNode();
};

// Physical node interface (result), should be used for automatic testing

class PResultNode {

public:
    PResultNode(PResultNode* left, PResultNode* right, LAbstractNode* p);
    virtual ~PResultNode();
    // returns number of attributes
    virtual size_t GetAttrNum() = 0;
    // prints tree
    virtual void Print(size_t indent) = 0;
    // used to get attribute info
    LAbstractNode* prototype;
    // returns error status and data, if possible
    virtual std::tuple<ErrCode, query_result_row> GetRecord();

protected:
    PResultNode* left;
    PResultNode* right;
    query_result data;

private:
    size_t record_position;
};
