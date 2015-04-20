#ifndef DISJUNCTION_CONVERTER_H
#define DISJUNCTION_CONVERTER_H

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "expression/predicate.h"
#include "expression/predicate_tree.h"
#include "expression/comparison_predicate.h"

namespace ac {
class DisjunctionConverter {
 public:
  static std::vector<std::vector<ComparisonPredicate> > convert2Disjunction(ac::Predicate* pred) {
    
    std::unordered_set<ComparisonPredicate> pred_set;
    std::unordered_map<ComparisonPredicate, bool> predValMap;

    std::queue<Predicate*> predQueue;
    predQueue.push(pred);
    
    // Get all the comparison predicate (atomic predicate).
    while (!predQueue.empty()) {
      Predicate *cur = predQueue.front();
      predQueue.pop();
      switch (cur->getPredicateType()) {
        case Predicate::kComparison: {
          ComparisonPredicate *compPred = dynamic_cast<ComparisonPredicate*>(cur);
          // predMap[*compPred] = compPred;
          pred_set.insert(*compPred);
          break;
        }
        case Predicate::kNegation: {
          // TODO here.
          break;
        }
        case Predicate::kConjunction: { 
          ConjunctionPredicate *conjunctionPred = dynamic_cast<ConjunctionPredicate*>(cur);
          for (auto item : conjunctionPred->dynamic_operand_list_) {
        	  predQueue.push(item);
          }
          // predQueue.push(pred);
          break;
        }
        case Predicate::kDisjunction: {
          DisjunctionPredicate *disjunctionPred = dynamic_cast<DisjunctionPredicate*>(cur);
          for (auto item : disjunctionPred->dynamic_operand_list_) {
        	  predQueue.push(item);
          }
          break;
        }
      }
    }
    std::vector<std::vector<ComparisonPredicate> > disjunction_predicates;
    getDisjunction(pred, predValMap, pred_set, disjunction_predicates);
    return (disjunction_predicates);

  } 

 private:
   void static getDisjunction(
      ac::Predicate* pred,
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
      }

      return;
    }
    ComparisonPredicate comp_pred = *(pred_set.begin());
    pred_val_map[comp_pred] = true;
    pred_set.erase(comp_pred);
    getDisjunction(pred, pred_val_map, pred_set, disjunction_predicates);
    pred_val_map[comp_pred] = false;
    getDisjunction(pred, pred_val_map, pred_set, disjunction_predicates);
    pred_set.insert(comp_pred);
    return;
  }
};

} // namespace ac
#endif
