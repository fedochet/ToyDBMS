#pragma once

#include "../basics.h"
#include "../interface.h"

class LSelectNode : public LAbstractNode {
public:
    LSelectNode(BaseTable& table, std::vector<PredicateInfo> predicates);
    // returns a reference to BaseTable
    BaseTable& GetBaseTable();
    // returns end status and next predicate (if exists)
    std::tuple<int, PredicateInfo> GetNextPredicate();
    // resets predicate iterator
    void ResetIterator();
    ~LSelectNode();
    std::vector<PredicateInfo> predicates;
private:
    size_t iteratorpos;
    BaseTable table;
};