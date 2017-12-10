#pragma once

#include "../interface.h"
#include "../../utils/utils.h"

class LJoinNode : public LAbstractNode {
public:
    // offsets are defined as "TableName.AttributeName" so, ensure there is no duplicates
    LJoinNode(LAbstractNode* left, LAbstractNode* right, std::string offset1, std::string offset2, int memorylimit = 0);
    ~LJoinNode();

    join_offset GetLeftOffset() {
        return std::make_pair(FindColumnOffset(left->fieldNames), left_offset);
    }

    join_offset GetRightOffset() {
        return std::make_pair(FindColumnOffset(right->fieldNames), right_offset);
    }

    size_t FindColumnOffset(const std::vector<std::vector<std::string>> &names) const {
        for (size_t i = 0; i < names.size(); i++) {
            size_t lpos1 = utils::find(names[i], left_offset);
            size_t lpos2 = utils::find(names[i], right_offset);

            if (lpos1 < names[i].size() || lpos2 < names[i].size()) {
                return i;
            }
        }

        throw std::runtime_error(
            std::string("Cannot joins by column named ") + left_offset + " or " + right_offset
        );

    }


    // attributes to perform equi-joins on
    std::string left_offset, right_offset;
    // maximum number of records permitted to present inside physical node
    int memorylimit;
};