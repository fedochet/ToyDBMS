#pragma once

#include <tuple>
#include "../projection/pprojectnode.h"
#include "../cross_product/pcrossproductnode.h"
#include "../joins/pjoinnode.h"
#include "../select/pselectnode.h"
#include "../../interface/select/lselectnode.h"
#include "../../interface/joins/ljoinnode.h"
#include "../../interface/joins/lsortmergejoin.h"
#include "../joins/psortmergejoinnode.h"

// Here be rewriter and optimizer
PResultNode* QueryFactory(LAbstractNode* node){
  if (auto* selectNode = dynamic_cast<LSelectNode*>(node)){
    std::vector<Predicate> p = selectNode->predicates;
    return new PSelectNode(selectNode, p);
  }

  if (auto* joinNode = dynamic_cast<LJoinNode*>(node)) {
    auto* leftPNode = dynamic_cast<PGetNextNode*>(QueryFactory(joinNode->GetLeft()));
    auto* rightPNode = dynamic_cast<PGetNextNode*>(QueryFactory(joinNode->GetRight()));

    return new PJoinNode(joinNode, leftPNode, rightPNode);
  }

  if (auto* sortJoinNode = dynamic_cast<LSortMergeJoinNode*>(node)) {
    auto* leftPNode = dynamic_cast<PGetNextNode*>(QueryFactory(sortJoinNode->GetLeft()));
    auto* rightPNode = dynamic_cast<PGetNextNode*>(QueryFactory(sortJoinNode->GetRight()));

    return new PSortMergeJoinNode(sortJoinNode, leftPNode, rightPNode);
  }


  if (auto* l_cross_product_node = dynamic_cast<LCrossProductNode*>(node)) {
    auto* rres = dynamic_cast<PGetNextNode*>(QueryFactory(node->GetRight()));
    auto* lres = dynamic_cast<PGetNextNode*>(QueryFactory(node->GetLeft()));

    return new PCrossProductNode(lres, rres, l_cross_product_node);
  }

  if (auto l_project_node = dynamic_cast<LProjectNode*>(node)) {
    auto next = dynamic_cast<PGetNextNode*>(QueryFactory(l_project_node->GetLeft()));
    return new PProjectNode(next, l_project_node);
  }

  throw std::runtime_error("Unknow node type!");

}

void ExecuteQuery(PResultNode* query){
  std::tuple<ErrCode, std::vector<Value>> res;
  res = query->GetRecord();
  ErrCode ec = std::get<0>(res);
  std::vector<Value> vals = std::get<1>(res);
  while(ec == EC_OK){
    for (size_t i = 0; i < query->GetAttrNum(); i++){
      if(vals[i].vtype == VT_INT)
        std::cout << vals[i].vint << " ";
      else if(vals[i].vtype == VT_STRING)
        std::cout << vals[i].vstr << " ";
    }
    printf("\n");
    res = query->GetRecord();
    ec = std::get<0>(res);
    vals = std::get<1>(res);
  }

}

