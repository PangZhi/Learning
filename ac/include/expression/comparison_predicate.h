#ifndef COMPARISON_PREDICATE_H
#define COMPARISON_PREDICATE_H

#include <string>
class ComparisonPredicate : public Predicate {
 public:
  ComparisonPredicate(const Comparison &comp,
                      const string &lop,
                      const AttrVal &rop);
  ~ComparisonPredicate() {}

  Predicate* clone() const;

  PredicateType getPredicateType() const {
    return kComparison;
  }

 private:
  const Comparison comparison_;
  std::string lop_;
  util::AttrVal rop_;

  void initHelper(bool own_children);

}; // class ComparisonPredicate

#endif // COMPARISON_PREDICATE_H
