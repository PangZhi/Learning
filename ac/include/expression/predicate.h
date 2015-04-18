#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <unordered_map>
#include "attrval.h"

namespace ac {

class ComparisonPredicate;

extern std::unordered_map<std::string, util::AttrVal> valueMap;

class Predicate {
 public:
  enum PredicateType {
    kTrue = 0,
    kFalse,
    kComparison,
    kNegation,
    kConjunction,
    kDisjunction,
  };

  Predicate() : has_static_result_(false), static_result_(false) {}

  virtual ~Predicate() {}

  virtual PredicateType getPredicateType() const = 0;

  virtual bool hasStaticResult() const {
    return has_static_result_;
  }

  virtual bool getStaticResult() const {
    return static_result_;
  }

  virtual Predicate* clone() const = 0;
  virtual bool eval() = 0;
  virtual bool eval(std::unordered_map<ComparisonPredicate, bool>& pred_val_map) = 0;
  protected:
    bool has_static_result_;
    bool static_result_;
}; // class Predicate
} // namespace ac
 
#endif // PREDICATE_H
