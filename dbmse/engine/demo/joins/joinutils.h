#pragma once

#include "pjoinnode.h"

namespace utils {

    template<class LogicalJoinNode>
    struct TableRowMerger {
        explicit TableRowMerger(LogicalJoinNode* p)
            : prototype(p) {};

        query_result_row MergeRows(const query_result_row &left_row, const query_result_row &right_row) const {

            query_result_row tmp;
            for (size_t k = 0; k < prototype->GetLeft()->fieldNames.size(); k++) {
                if (k != prototype->GetLeftOffset().first) {
                    tmp.push_back(left_row[k]);
                }
            }

            for (size_t k = 0; k < prototype->GetRight()->fieldNames.size(); k++) {
                if (k != prototype->GetRightOffset().first) {
                    tmp.push_back(right_row[k]);
                }
            }

            tmp.push_back(left_row[prototype->GetLeftOffset().first]);
            return tmp;
        }

    private:
        LogicalJoinNode* prototype;
    };
}