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
//      1) support for restricting physical join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#ifndef PGETNEXTNODE_H
#define PGETNEXTNODE_H
#include <vector>
#include "../interface/interface.h"

static const size_t BLOCK_SIZE = 2;

typedef std::vector<Value> query_result_row;
typedef std::vector<query_result_row> query_result;

class PGetNextNode : public PResultNode{
  public:
    explicit PGetNextNode(LAbstractNode* p, PResultNode* left, PResultNode* right);
    // internal way to transfer data
    virtual query_result GetAllData();
    virtual query_result GetNextBlock();
    // getting access to data
    virtual void Initialize();
    // get number of attributes
    size_t GetAttrNum() override;
    // returns error status and data, if possible
    // TODO reimplement it
    virtual std::tuple<ErrCode, std::vector<Value>> GetRecord();

};

#endif // PGETNEXTNODE_H
