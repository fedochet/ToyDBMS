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
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#include <typeinfo>
#include <iostream>
#include <vector>
#include <tuple>
#include "../interface/interface.h"
#include "query_api/query_api.h"

int main(){

  {
    std::cout << "Starting demo" << std::endl;
    std::cout << "Query1: plain select" << std::endl;
    BaseTable bt1 = BaseTable("table1");
    std::cout << bt1;
    LAbstractNode* n1 = new LSelectNode(bt1, {});
    PResultNode* q1 = QueryFactory(n1);
    q1->Print(0);
    ExecuteQuery(q1);
    delete n1;
    delete q1;
  }
  
  {
    std::cout << std::endl << "Query2: nested loop join" << std::endl;
    BaseTable bt1 = BaseTable("table1");
    BaseTable bt2 = BaseTable("table2");
    LJoinNode* join_node = new LJoinNode(new LSelectNode(bt1, {}), new LSelectNode(bt2, {}), "table1.groups",
                                         "table2.id2");
    PResultNode* join_physical_node = QueryFactory(join_node);

    join_physical_node->Print(0);
    ExecuteQuery(join_physical_node);

    std::cout << std::endl;
    delete join_physical_node;
    delete join_node;
  }

  {
    std::cout << std::endl << "Query2: simple equi-join" << std::endl;
    BaseTable bt1 = BaseTable("table1");
    BaseTable bt2 = BaseTable("table2");
    std::cout << bt1;
    std::cout << bt2;
    std::vector<Predicate> predicates = {
        Predicate(PT_EQUALS, VT_STRING, 1, 0, "cero")
    };
    LAbstractNode* n1 = new LSelectNode(bt1, predicates);
    LAbstractNode* n2 = new LSelectNode(bt2, {});
//    LJoinNode* n3 = new LJoinNode(n1, n2, "table1.id", "table2.id2", 666);
    auto* n3 = new LCrossProductNode(n1, n2);
    PResultNode* q1 = QueryFactory(n3);
    std::cout << std::endl;
    q1->Print(0);
    ExecuteQuery(q1);
    delete n3;
    delete q1;
  }

  {
    std::cout << std::endl;
    BaseTable bt1 = BaseTable("table1");
    BaseTable bt2 = BaseTable("table2");
    std::cout << bt1;
    std::cout << bt2;
    std::vector<Predicate> predicates = {
        Predicate(PT_EQUALS, VT_STRING, 1, 0, "cero")
    };
    LAbstractNode* n1 = new LSelectNode(bt1, predicates);
//    LAbstractNode* n2 = new LSelectNode(bt2, {});

    LProjectNode* p1 = new LProjectNode(n1, {"table1.description", "table1.frequency"});
    PResultNode* q1 = QueryFactory(p1);
    q1->Print(0);
    ExecuteQuery(q1);

    delete q1;
    delete p1;
  }

}
