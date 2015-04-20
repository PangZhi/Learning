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

  bool operator== (const ComparisonPredicate& p) const {
    return (this->comparison() == p.comparison() 
         && this->lop() == p.lop()
            && this->rop() == p.rop());
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

  bool eval(std::unordered_map<ComparisonPredicate, bool>&) override;
  
  Comparison comparison() const {
    return comparison_;
  }

  std::string lop() const {
    return lop_;
  }

  util::AttrVal rop() const {
    return rop_;
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
  template <>
  struct hash<ac::ComparisonPredicate> {
    size_t operator() (const ac::ComparisonPredicate& compPred) const {
      //size_t  first_hash;
      size_t first_hash = hash<int>()(compPred.comparison());
      size_t second_hash = hash<std::string>()(compPred.lop());
      size_t third_hash = hash<util::AttrVal>()(compPred.rop());
      const size_t kMul = 0x9ddfea08eb382d69ULL;
      size_t a = (first_hash ^ second_hash) * kMul;
      a ^= (a >> 47);
      size_t b = (second_hash ^ a) * kMul;
      b ^= (b >> 47);
      b *= kMul;
      size_t c = (third_hash ^ b) * kMul;
      c ^= (c >> 47);
      return c;
    }
  };
} // namespace std


#endif // COMPARISON_PREDICATE_H
