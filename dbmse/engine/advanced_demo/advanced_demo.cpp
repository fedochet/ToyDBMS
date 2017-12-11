#include "../interface/basics.h"
#include "../interface/interface.h"
#include "../demo/query_api/query_api.h"

using namespace std;

int main() {
  {
    BaseTable bt1 = BaseTable("USER");
    auto select_node = new LSelectNode(bt1, {});
    auto p_node = QueryFactory(select_node);

    ExecuteQuery(p_node);
    cout << endl;

    delete select_node;
    delete p_node;

    cout << endl;
  }

  {
    BaseTable bt1 = BaseTable("ORDER");
    auto select_node = new LSelectNode(bt1, {});
    auto p_node = QueryFactory(select_node);

    ExecuteQuery(p_node);
    cout << endl;

    delete select_node;
    delete p_node;

    cout << endl;
  }

  {
    BaseTable bt1 = BaseTable("PRODUCT");
    auto select_node = new LSelectNode(bt1, {});
    auto p_node = QueryFactory(select_node);

    ExecuteQuery(p_node);
    cout << endl;

    delete select_node;
    delete p_node;

    cout << endl;
  }

  {
    BaseTable bt1 = BaseTable("USER");
    BaseTable bt2 = BaseTable("ORDER");
    auto user_select = new LSelectNode(bt1, {});
    auto order_select = new LSelectNode(bt2, {});

    auto user_order_join = new LNestedLoopJoinNode(user_select, order_select, "USER.email", "ORDER.customer_email");
    auto p_node = QueryFactory(user_order_join);

    p_node->Print(0);
    ExecuteQuery(p_node);

    delete user_order_join;
    delete p_node;

    cout << endl;
  }

  {
    BaseTable bt1 = BaseTable("USER");
    BaseTable bt2 = BaseTable("ORDER");
    BaseTable bt3 = BaseTable("PRODUCT");
    auto user_select = new LSelectNode(bt1, {PredicateInfo(PT_GREATERTHAN, "age", Value(23))});
    auto order_select = new LSelectNode(bt2, {});
    auto product_select = new LSelectNode(bt3, {});

    auto user_order_join = new LNestedLoopJoinNode(user_select, order_select, "USER.email", "ORDER.customer_email");
    auto user_order_product_join = new LHashJoinNode(user_order_join, product_select, "ORDER.product_id", "PRODUCT.id");
    auto projection = new LProjectNode(user_order_product_join, {"USER.firstname", "USER.lastname", "PRODUCT.name"});
    auto p_node = QueryFactory(projection);

    p_node->Print(0);
    ExecuteQuery(p_node);

    delete projection;
    delete p_node;

    cout << endl;
  }

  {
    cout << "Simple sort merge join test" << endl;

    BaseTable bt1 = BaseTable("USER");
    BaseTable bt2 = BaseTable("PRODUCT");

    auto user_select = new LSelectNode(bt1, {});
    auto products_select = new LSelectNode(bt2, {});
    auto sorted_join = new LSortMergeJoinNode(user_select, products_select, "USER.id", "PRODUCT.id"); // no sense
    auto p_node = QueryFactory(sorted_join);

    p_node->Print(0);
    ExecuteQuery(p_node);

    delete sorted_join;
    delete p_node;

    cout << endl;
  }

  {
    cout << "More advanced merge join test" << endl;

    BaseTable bt1 = BaseTable("CITY");
    BaseTable bt2 = BaseTable("SIGHT");

    auto city_select = new LSelectNode(bt1, {});
    auto sight_select = new LSelectNode(bt2, {});
    auto sorted_join = new LSortMergeJoinNode(city_select, sight_select, "CITY.name", "SIGHT.city_name");
    auto p_node = QueryFactory(sorted_join);

    p_node->Print(0);
    ExecuteQuery(p_node);

    delete sorted_join;
    delete p_node;

    cout << endl;
  }

  {
    cout << "Even More advanced merge join test: each with each in one city" << endl;

    BaseTable bt1 = BaseTable("SIGHT");
    BaseTable bt2 = BaseTable("SIGHT");

    auto sight_select = new LSelectNode(bt1, {});
    auto sight_select1 = new LSelectNode(bt2, {});
    auto sorted_join = new LSortMergeJoinNode(sight_select, sight_select1, "SIGHT.city_name", "SIGHT.city_name");
    auto p_node = QueryFactory(sorted_join);

    p_node->Print(0);
    ExecuteQuery(p_node);

    delete sorted_join;
    delete p_node;

    cout << endl;
  }

  return 0;
}
