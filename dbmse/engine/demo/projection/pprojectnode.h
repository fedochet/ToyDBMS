#pragma once

#include "../pgetnextnode.h"
#include "../../interface/projection/lprojectnode.h"

class PProjectNode : public PGetNextNode {
public:
    explicit PProjectNode(PGetNextNode* from, LProjectNode* lProjectNode);

    void Print(size_t indent) override;

    query_result GetNextBlock() override;
    void Rewind() override;

    size_t GetAttrNum() override;

    virtual ~PProjectNode();
};