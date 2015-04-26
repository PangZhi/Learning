#include "expression/comparison_predicate.h"

namespace ac {

ComparisonPredicate::ComparisonPredicate(const Comparison& comp,
                                         const std::string& attr_str,
                                         const util::AttrVal& val,
                                         const ComparisonType& comp_type) 
    : comparison_(comp),
      val_(val),
      comparison_type_(comp_type) {
  switch(comparison_type_) {
    case kUser2Val:
      user_attr_ = attr_str;
      break;
    case kCol2Val:
      col_attr_ = attr_str;
  }
}

ComparisonPredicate::ComparisonPredicate(const Comparison& comp,
                                         const std::string& user_attr,
                                         const std::string& col_attr)
    : comparison_(comp),
      comparison_type_(kUser2Col),
      user_attr_(user_attr),
      col_attr_(col_attr) {}


Predicate* ComparisonPredicate::clone() const {
  switch (comparison_type_) {
    case kUser2Col:
      return new ComparisonPredicate(comparison_,
                                     user_attr_,
                                     col_attr_);
    case kUser2Val:
      return new ComparisonPredicate(comparison_,
                                     user_attr_,
                                     val_,
                                     kUser2Val);
    case kCol2Val:
      return new ComparisonPredicate(comparison_,
                                     col_attr_,
                                     val_,
                                     kCol2Val);
    default:
      return nullptr;
  }
}

bool ComparisonPredicate::operator== (const ComparisonPredicate& p) const {
  return (this->Serialize() == p.Serialize());
}

std::string ComparisonPredicate::Serialize() const {
  std::string ret;
  ret.append(std::to_string(comparison_));
  ret.append("u:").append(user_attr_);
  ret.append("c:").append(col_attr_);
  ret.append(val_.serialize());
  ret.append(std::to_string(comparison_type_));
  return ret;
}

bool ComparisonPredicate::eval() {
  return true;
}

bool ComparisonPredicate::eval(std::unordered_map<ComparisonPredicate, bool>& pred_val_map) {
  return pred_val_map[*this];
}
}  // namespace quickstep
