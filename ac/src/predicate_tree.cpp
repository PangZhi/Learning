#include "expression/predicate_tree.h"

namespace ac {

  Predicate* NegationPredicate::clone() const {
    return new NegationPredicate(operand_->clone());
  }

  Predicate* NegationPredicate::NegationPredicate(Predicate* operand) {
    if (operand->getPredicateType() == kNegation) {
      NegationPredicate *negation_operand = static_cast<NegationPredicate*>(operand);
      Predicate *inner = negation_operand->operand_.release();
      delete negation_operand;
      return inner;
    } else {
      return new NegationPredicate(operand);
    }
  }  

  Predicate* ConjunctionPredicate::clone() const {
    ConjunctionPredicate *clone = new ConjunctionPredicate();

    for (std::vector<Predicate*>::const_iterator it = static_operand_list_.,begin();
        it != static_operand_list_.end();
        ++it) {
      clone->addPredicate(it->clone);
    }
    
    for (std::vector<Predicate*>::const_iterator it = dynamic_operand_list_.begin();
        it != dynamic_operand_list_.end();
        ++it) {
      clone->addPredicate(it->clone());
    }
    return clone;
  }
  
  void ConjunctionPredicate::addPredicate(Predicate* operand) {
    if (operand->getPredicateType() == kConjunction) {
      ConjunctionPredicate *conjunction_operand = static_cast<ConjunctionPredicate*>(operand);

      std::vector<Predicate*>::iterator checkpos;
      
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
        processStaticOperand(*checkpos);
        ++checkpos;
      }

      if (!conjunction_operand->dynamic_operand_list_.empty()) {
        dynamic_operand_list_.splice(dynamic_operand_list_.end(), conjunction_operand->dynamic_operand_list_);
        processDynamicOperand();
      }

      delete conjunction_operand;
    } else {
      if (operand->hasStaticResult()) {
        static_operand_list_.push_back(operand);
        processStaticOperand(*operand);
      } else {
        dynamic_operand_list_.push_back(operand);
        processDynamicOperand();
      }
    }
  }

  void ConjunctionPredicate::processStaticOperand(const Predicate& operand) {
    if (!operand.getStaticResult()) {
      has_static_result = true;
      static_result = false;
      
    }
  }

  // TODO: simpify here. Don't need static result. All result should be
  // dynamic.
  void ConjunctionPredicate::processDynamicOperand() {
    
  }


} // namespace ac
