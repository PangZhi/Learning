#include "expression/comparison_predicate.h"

namespace ac {

ComparisonPredicate::ComparisonPredicate(const Comparison &comparison,
                                         const std::string &lop,
                                         const util::AttrVal &rop)
    : comparison_(comparison),
      lop_(lop),
      rop_(rop) {
  // initHelper(false);
}

Predicate* ComparisonPredicate::clone() const {
  return new ComparisonPredicate(comparison_,
                                 lop_,
                                 rop_);
}

bool ComparisonPredicate::eval() {
  return true;
}

}  // namespace quickstep
