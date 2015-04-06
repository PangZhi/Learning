#ifndef PREDICATE_TREE_H
#define PREDICATE_TREE_H

#include <memory>
#include <vector>

#include "predicate.h"

namespace ac {

class NegationPredicate : public Predicate {
 public:
  explicit NegationPredicate(Predicate *operand) : operand_ (operand) {
    initHelper();
  }

  ~NegationPredicate() {}

  static Predicate* NegationPredicate(Predicate *operand);
  Predicate* clone() const;

  PredicateType getPredicateType() const {
    return kNegation;
  }

 private:
  void initHelper();
  std::unique_ptr<Predicate> operand_;
}; // class NegationPredicate

class PredicateWithList : public Predicate {
 public:
  PredicateWithList() {}
  virtual ~PredicateWithList() {}
  virtual void addPredicate(Predicate *operand)  = 0;

 protected:
  std::vector<Predicate*> static_operand_list_;
  std::vector<Predicate*> dynamic_operand_list_;

}; // class PredicateWithList

class ConjunctionPredicate : public PredicateWithList {
 public:
  // Strange here
  ConjunctionPredicate() : PredicateWithList() {}
  Predicate *clone() const;

  PredicateType getPredicateType() const {
    return kConjunction;
  }

  void addPredicate(Predicate *operand);

 private:
  void processStaticOperand(const Predicate &operand);
  void processDynamicOperand();
}; // class ConjunctionPredicate

class DisjunctionPredicate : public PredicateWithList {
 public:
  DisjunctionPredicate() : PredicateWithList() {}

  Predicate *clone() const;

  PredicateType getPredicateType() const {
    return kDisjunction;
  }

  void addPredicate(Predicate *operand);

 private:
  void processStaticOperand(const Predicate& operand);
  void processDynamicOperand();
}; // class DisjunctionPredicate

} // namespace ac
#endif 
