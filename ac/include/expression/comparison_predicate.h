#ifndef COMPARISON_PREDICATE_H
#define COMPARISON_PREDICATE_H

#include <string>

#include "expression/comparison.h"
#include "expression/predicate.h"
#include "attrval.h"

namespace ac {
class ComparisonPredicate : public Predicate {
 public:
  enum ComparisonType {
    kUser2Val = 0,
    kCol2Val,
    kUser2Col,
  };
  
  /**
  ComparisonPredicate(const Comparison &comp,
                      const std::string &lop,
                      const util::AttrVal &rop);
  **/

  ComparisonPredicate(const Comparison& comp,
                      const std::string& attr_str,
                      const util::AttrVal& val,
                      const ComparisonType& comp_type);
 
  ComparisonPredicate(const Comparison& comp,
                      const std::string& user_attr,
                      const std::string& col_attr);

  ~ComparisonPredicate() {}

  Predicate* clone() const override;

  PredicateType getPredicateType() const override {
    return kComparison;
  }

  bool operator== (const ComparisonPredicate& p) const; 
  
  bool eval() override;

  bool eval(std::unordered_map<ComparisonPredicate, bool>&) override;

  serialization::Predicate GetProto() const;

  inline Comparison comparison() const {
    return comparison_;
  }

  inline std::string user_attr() const {
    return user_attr_;
  }

  inline std::string col_attr() const {
    return col_attr_;
  } 

  inline ComparisonType comparison_type() const {
    return comparison_type_;
  }

  inline util::AttrVal val() const {
    return val_;
  }

  std::string Serialize() const;

  std::string GetLiteralString() const;

 private:
  Comparison comparison_;
  std::string user_attr_;
  std::string col_attr_; 
  util::AttrVal val_;
  ComparisonType comparison_type_;

  void initHelper(bool own_children);

}; // class ComparisonPredicate
} // namespace ac

namespace std {
  template <>
  struct hash<ac::ComparisonPredicate> {
    size_t operator() (const ac::ComparisonPredicate& compPred) const {
      return hash<std::string>()(compPred.Serialize());
    }
  };
} // namespace std


#endif // COMPARISON_PREDICATE_H
