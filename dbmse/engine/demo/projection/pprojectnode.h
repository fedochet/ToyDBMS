#pragma once

#include "../pgetnextnode.h"

class PProjectNode : public PGetNextNode {
public:
    explicit PProjectNode(PGetNextNode* from, LProjectNode* lProjectNode);

    void Print(size_t indent) override;

    std::vector<std::vector<Value>> GetNextBlock() override;

    size_t GetAttrNum() override;

    virtual ~PProjectNode();
};