#include <map>
#include "../../utils/bd_utils.h"
#include "../../interface/joins/labstractjoinnode.h"
#include "joinutils.h"

struct PHashJoinNode : PGetNextNode {
    PHashJoinNode(LAbstractJoinNode* p, PGetNextNode* left, PGetNextNode* right);

    void Print(size_t indent) override;

    ~PHashJoinNode() override;

    query_result GetNextBlock() override;

    void Rewind() override;

    size_t GetAttrNum() override;

private:
    join_offset left_join_offset;
    join_offset right_join_offset;

    utils::TableRowMerger merger;
    utils::BlockIterator left_iterator;
    utils::BlockIterator right_iterator;

    std::map<Value, query_result> hash_table;
    size_t hash_table_block_pos {0};

    void BuildHashTable();
};
