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
  static std::vector<std::vector<ComparisonPredicate*> > convert2Disjunction(const ac::Predicate* pred) {
    
    std::unordered_set<ComparisonPredicate> pred_set;
    std::unordered_map<ComparisonPredicate, bool> predValMap;

    std::queue<Predicate*> predQueue;
    predQueue.push_back(pred);
    
    // Get all the comparison predicate (atomic predicate).
    while (!predQueue.empty()) {
      Predicate *cur = predQueue.front();
      switch (cur->getPredicateType()) {
        case kComparison: 
          ComparisonPredicate *compPred = dynamic_cast<ComparisonPredicate*>(pred);
          // predMap[*compPred] = compPred;
          pred_set.insert(*compPred);
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
    std::vector<std::vector<ComparisonPredicate> > disjunction_predicates;
    getDisjunction(pred, predValMap, pred_set, disjunction_predicates)
    return (disjunction_predicates);

  } 

 private:
   convert2Disjunction::getDisjunction(
      const ac::Predicate* pred,
      std::unordered_map<ComparisonPredicate, bool>& pred_val_map,
      std::unordered_set<ComparisonPredicate>& pred_set,
      std::vector<std::vector<ComparisonPredicate> >& disjunction_predicates) {
    if (pred_set.empty()) {
      if (pred->eval(pred_val_map)) {
        std::vector<ComparisonPredicate> disjunctions;
        for (auto iter = pred_val_map.begin(); iter != pred_val_map.end(); ++iter) {
          if (iter->second == true) {
            disjunctions.push_back(iter->first);
          }
        }
        disjunction_predicates.push_back(disjunctions);
        return;
      }                                      
    }
    ComparisonPredicate comp_pred = *(*pred_set.begin());
    pred_val_map[comp_pred] = true;
    pred_set.erase(comp_pred);
    getDisjunction(pred, pred_val_map, pred_set, disjunction_predicates);
    pred_val_map[com_pred] = false;
    getDisjunction(pred, pred_val_map, pred_set, disjunction_predicates);
    pred_set.insert(comp_pred);
    return;
  }
};

} // namespace ac
#endif
