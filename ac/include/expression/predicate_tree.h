#ifndef PREDICATE_TREE_H
#define PREDICATE_TREE_H

#include <memory>
#include <list>

#include "expression/predicate.h"

namespace ac {

class NegationPredicate : public Predicate {
 public:
  explicit NegationPredicate(Predicate *operand) : operand_ (operand) {
    // initHelper();
  }

  ~NegationPredicate() {}

  static Predicate* NegatePredicate(Predicate *operand);
  Predicate* clone() const override;

  PredicateType getPredicateType() const override {
    return kNegation;
  }

  // Currently not useful.
  /**
  bool hasStaticResult() const override {
    return false;
  }

  bool getStaticResult() const override {
    return false;
  }
  */

  bool eval() override;

  bool eval(std::unordered_map<ComparisonPredicate, bool>&) override;
 private:
  void initHelper();
  std::unique_ptr<Predicate> operand_;
  // bool has_static_result_;
  // bool static_result_;

}; // class NegationPredicate

class PredicateWithList : public Predicate {
 public:
  PredicateWithList() {}
  virtual ~PredicateWithList() {}
  virtual void addPredicate(Predicate *operand)  = 0;

  // Currently not useful.
  /**bool hasStaticResult() const override() {
    return false; 
  }

  bool getStaticResult() const override() {
    return false;
  }
  */

  virtual bool eval() = 0;
  virtual bool eval(std::unordered_map<ComparisonPredicate, bool>&) = 0;
 //protected:
  std::list<Predicate*> static_operand_list_;
  std::list<Predicate*> dynamic_operand_list_;

  // bool has_static_result_;
  // bool static_result_;
}; // class PredicateWithList

class ConjunctionPredicate : public PredicateWithList {
 public:
  // Strange here
  ConjunctionPredicate() : PredicateWithList() {}
  Predicate *clone() const override;

  PredicateType getPredicateType() const {
    return kConjunction;
  }

  void addPredicate(Predicate *operand) override;

  bool eval() override;
  bool eval(std::unordered_map<ComparisonPredicate, bool>&);

 private:
  void processStaticOperand(const Predicate &operand);
  void processDynamicOperand();
}; // class ConjunctionPredicate

class DisjunctionPredicate : public PredicateWithList {
 public:
  DisjunctionPredicate() : PredicateWithList() {}

  Predicate *clone() const override;

  PredicateType getPredicateType() const override {
    return kDisjunction;
  }

  void addPredicate(Predicate *operand);
  bool eval() override;
  bool eval(std::unordered_map<ComparisonPredicate, bool>&);

 private:
  void processStaticOperand(const Predicate& operand);
  void processDynamicOperand();
}; // class DisjunctionPredicate

} // namespace ac
#endif 
