//
// Created by roman on 10.12.17.
//

#include "labstractjoinnode.h"
#include "../../utils/utils.h"


LAbstractJoinNode::LAbstractJoinNode(LAbstractNode* left,
                                     LAbstractNode* right,
                                     std::string left_offset,
                                     std::string right_offset)
    : LAbstractNode(left, right), left_offset(left_offset), right_offset(right_offset) {

    using namespace utils;

    name_aliases match;
    ValueType vt;
    COLUMN_SORT cs;
    for (size_t i = 0; i < left->fieldNames.size(); i++) {
        for (size_t j = 0; j < right->fieldNames.size(); j++) {
            name_aliases left_name_aliases = left->fieldNames[i];
            name_aliases right_name_aliases = right->fieldNames[j];

            if (contains(left_name_aliases, left_offset) && contains(right_name_aliases, right_offset)) {
                match = left_name_aliases;
                append_to_back(match, right_name_aliases);
                vt = left->fieldTypes[i];
                cs = left->fieldOrders[i];
            } else if (contains(left_name_aliases, right_offset) && contains(right_name_aliases, left_offset)) {
                match = left_name_aliases;
                append_to_back(match, right_name_aliases);
                vt = left->fieldTypes[i];
                cs = left->fieldOrders[i];
            }
        }
    }

    for (size_t i = 0; i < left->fieldNames.size(); i++) {
        name_aliases l = left->fieldNames[i];
        if (!contains(l, left_offset) && !contains(l, right_offset)) {
            fieldNames.push_back(l);
            fieldTypes.push_back(left->fieldTypes[i]);
            fieldOrders.push_back(left->fieldOrders[i]);
        }
    }

    for (size_t i = 0; i < right->fieldNames.size(); i++) {
        name_aliases r = right->fieldNames[i];
        if (!contains(r, left_offset) && !contains(r, right_offset)) {
            fieldNames.push_back(r);
            fieldTypes.push_back(right->fieldTypes[i]);
            fieldOrders.push_back(right->fieldOrders[i]);
        }
    }

    fieldNames.push_back(match);
    fieldTypes.push_back(vt);
    fieldOrders.push_back(cs);

}

join_offset LAbstractJoinNode::GetLeftOffset() {
    return std::make_pair(FindColumnOffset(left->fieldNames), left_offset);
}

join_offset LAbstractJoinNode::GetRightOffset() {
    return std::make_pair(FindColumnOffset(right->fieldNames), right_offset);
}

size_t LAbstractJoinNode::FindColumnOffset(const std::vector<std::vector<std::string>> &names) const {
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