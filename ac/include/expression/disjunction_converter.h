#ifndef DISJUNCTION_CONVERTER_H
#define DISJUNCTION_CONVERTER_H

#include <queue>
#include <unordered_map>
#include <vector>

#include "expression/predicate.h"
#include "expression/predicate_tree.h"
#include "expression/comparison_predicate.h"

namespace ac {
class DisjunctionConverter {
 public:
  static std::vector<std::vector<ac::ComparisonPredicate*> > convert2Disjunction(const ac::Predicate* pred) {
    std::unordered_map<ComparisonPredicate, ComparisonPredicate*> predMap;
    std::queue<Predicate*> predQueue;
    predQueue.push_back(pred);

    while (!predQueue.empty()) {
      Predicate *cur = predQueue.front();
      switch (cur->getPredicateType()) {
        case kComparison: 
          ComparisonPredicate *compPred = dynamic_cast<ComparisonPredicate*>(pred);
          predMap[*compPred] = compPred;
          break;
        case kNegation: 
          // TODO here.
          break;
        case kConjunction: 
          ConjunctionPredicate *conjunctionPred = dynamic_cast<ConjunctionPredicate*>(pred);
          predQueue.push_back(pred);
          break;
        case kDisjunction: 
          DisjunctionPredicate *disjunctionPred = dynamic_cast<DisjunctionPredicate*>(pred);
          predQueue.push_back(pred);
          break;
      }
    }
    // Use BFS to scan all the atomic predicates.
    
  } 

};

} // namespace ac
#endif
