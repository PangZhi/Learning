#ifndef COMPARISON_PREDICATE_H
#define COMPARISON_PREDICATE_H

#include <string>

#include "expression/comparison.h"
#include "expression/predicate.h"
#include "attrval.h"

namespace ac {
class ComparisonPredicate : public Predicate {
 public:
  ComparisonPredicate(const Comparison &comp,
                      const std::string &lop,
                      const util::AttrVal &rop);
  ~ComparisonPredicate() {}

  Predicate* clone() const override;

  PredicateType getPredicateType() const override {
    return kComparison;
  }

  /**bool hasStaticResult() const override {
    return false;
  }

  bool getStaticResult() const override {
    return false;
  }
  */
  
  /**
  bool eval(const AttrVal& lval) const;
  */

  bool eval() override;

  Comparison comparison() {
    return comparison_;
  }

  std::string lop() {
    return lop;
  }

  util::AttrVal rop() {
    return rop;
  }

 private:
  const Comparison comparison_;
  std::string lop_;
  util::AttrVal rop_;
  // Currently not useful.
  // bool static_result_;

  void initHelper(bool own_children);

}; // class ComparisonPredicate
} // namespace ac

namespace std {
  struct hash<ac::ComparisonPredicate> {
    size_t operator() (const ac::ComparisonPredicate& compPred) const {
      size_t  first_hash;
    }
  }
} // namespace std


#endif // COMPARISON_PREDICATE_H
