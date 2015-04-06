#ifndef PREDICATE_H
#define PREDICATE_H

namespace ac {
class Predicate {
 public:
  enum PredicateType {
    kTrue = 0;
    kFalse,
    kComparison,
    kNegation,
    kConjunction,
    kDisjunction,
  };

  virtual ~Predicate() {}

  virtual PredicateType getPredicateType() const = 0;

  protected:
    Predicate() {}
}; // class Predicate
} // namespace ac
 
#endif // PREDICATE_H
