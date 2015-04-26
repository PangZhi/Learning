#include "expression/comparison.h"

namespace ac {

std::string GetComparisonStr(const Comparison& comp) {
  switch (comp) {
    case kEqual:
      return "=";
    case kGreater:
      return ">";
    case kLess:
      return "<";
    case kGE:
      return ">=";
    case kLE:
      return "<=";
  } 
}
} // namespace ac
