#include "expression/predicate_tree.h"
#include "expression/comparison_predicate.h"

namespace ac {
  Predicate* NegationPredicate::clone() const {
    return new NegationPredicate(operand_->clone());
  }

  Predicate* NegationPredicate::NegatePredicate(Predicate* operand) {
    if (operand->getPredicateType() == kNegation) {
      NegationPredicate *negation_operand = static_cast<NegationPredicate*>(operand);
      Predicate *inner = negation_operand->operand_.release();
      delete negation_operand;
      return inner;
    } else {
      return new NegationPredicate(operand);
    }
  }  

  bool NegationPredicate::eval() {
    return true;
  }

  bool NegationPredicate::eval(std::unordered_map<ComparisonPredicate, bool>& pred_val_map) {
    return (!this->operand_->eval(pred_val_map));
  }

  Predicate* ConjunctionPredicate::clone() const {
    ConjunctionPredicate *clone = new ConjunctionPredicate();

    for (std::list<Predicate*>::const_iterator it = static_operand_list_.begin();
        it != static_operand_list_.end();
        ++it) {
      clone->addPredicate((*it)->clone());
    }
    
    for (std::list<Predicate*>::const_iterator it = dynamic_operand_list_.begin();
        it != dynamic_operand_list_.end();
        ++it) {
      clone->addPredicate((*it)->clone());
    }
    return clone;
  }
  
  void ConjunctionPredicate::addPredicate(Predicate* operand) {
    if (operand->getPredicateType() == kConjunction) {
      ConjunctionPredicate *conjunction_operand = static_cast<ConjunctionPredicate*>(operand);

      std::list<Predicate*>::iterator checkpos;
      
      if (static_operand_list_.empty()) {
        static_operand_list_.splice(static_operand_list_.end(), conjunction_operand->static_operand_list_);
        checkpos = static_operand_list_.begin();
      } else {
        checkpos = static_operand_list_.end();
        --checkpos;
        static_operand_list_.splice(static_operand_list_.end(), conjunction_operand->static_operand_list_);
        ++checkpos;
      }

      while (checkpos != static_operand_list_.end()) {
        // processStaticOperand(*checkpos);
        ++checkpos;
      }

      if (!conjunction_operand->dynamic_operand_list_.empty()) {
        dynamic_operand_list_.splice(dynamic_operand_list_.end(), conjunction_operand->dynamic_operand_list_);
        // processDynamicOperand();
      }

      delete conjunction_operand;
    } else {
      if (operand->hasStaticResult()) {
        static_operand_list_.push_back(operand);
        // processStaticOperand(*operand);
      } else {
        dynamic_operand_list_.push_back(operand);
        // processDynamicOperand();
      }
    }
  }

/*
  void ConjunctionPredicate::processStaticOperand(const Predicate& operand) {
    if (!operand.getStaticResult()) {
      has_static_result = true;
      static_result = false;
      
    }
  }

  // TODO: simpify here. Don't need static result. All result should be
  // dynamic.
  void ConjunctionPredicate::processDynamicOperand() {
    if (has_static_result_) {
      if (static_result_) {
        has_static_result_ = false;
      }
    }    
  }
*/
bool ConjunctionPredicate::eval() {
  return true;
}

bool ConjunctionPredicate::eval(std::unordered_map<ComparisonPredicate, bool>& pred_val_map) {
  for (auto item : dynamic_operand_list_) {
    if (!item->eval(pred_val_map)) {
      return false;
    }
  }  
  return true;
}

Predicate* DisjunctionPredicate::clone() const {
  DisjunctionPredicate *clone = new DisjunctionPredicate();

  for (auto it = static_operand_list_.begin();
       it != static_operand_list_.end();
       ++it) {
         clone->addPredicate((*it)->clone());
       }

  for (auto it = dynamic_operand_list_.begin();
       it != dynamic_operand_list_.end();
       ++it) {
         clone->addPredicate((*it)->clone());
       }
  return clone;
}

void DisjunctionPredicate::addPredicate(Predicate *operand) {
  if (operand->getPredicateType() == kDisjunction) {
    DisjunctionPredicate *disjunction_operand = static_cast<DisjunctionPredicate*>(operand);

    std::list<Predicate*>::iterator checkpos;
    if (static_operand_list_.empty()) {
      static_operand_list_.splice(static_operand_list_.end(), disjunction_operand->static_operand_list_);
      checkpos = static_operand_list_.begin();
    } else {
      checkpos = static_operand_list_.end();
      --checkpos;
      static_operand_list_.splice(static_operand_list_.end(), disjunction_operand->static_operand_list_);
      ++checkpos;
    }

    while (checkpos != static_operand_list_.end()) {
      // processStaticOperand(*checkpos);
      ++checkpos;
    }

    if (!disjunction_operand->dynamic_operand_list_.empty()) {
      dynamic_operand_list_.splice(dynamic_operand_list_.end(), disjunction_operand->dynamic_operand_list_);
      // processDynamicOperand();
    }

    delete disjunction_operand;
  } else {
    if (operand->hasStaticResult()) {
      static_operand_list_.push_back(operand);
      // processStaticOperand(*operand);
    } else {
      dynamic_operand_list_.push_back(operand);
      // processDynamicOperand();
    }
  }
}

bool DisjunctionPredicate::eval() {
  return true;
}


bool DisjunctionPredicate::eval(std::unordered_map<ComparisonPredicate, bool>& pred_val_map) {
  for (auto item : dynamic_operand_list_) {
    if (item->eval(pred_val_map)) {
      return true;
    }
  }  
  return false;
}

void DisjunctionPredicate::processStaticOperand(const Predicate &operand) {
  /**
  if (operand.getStaticResult()) {
    has_static_result_ = true;
    static_result_ = true;
  } else if (fresh_) {
    has_static_result_ = true;
    static_result_ = false;
  }

  fresh_ = false;
  **/
}

void DisjunctionPredicate::processDynamicOperand() {
  /**
  if (has_static_result_ && !static_result_) {
    has_static_result_ = false;
  } else if (fresh_) {
    has_static_result_ = false;
  }

  fresh_ = false;
  */}

} // namespace ac
