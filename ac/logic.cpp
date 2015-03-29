#include "logic.h"

namespace util {

bool Logic::evaluate() {
  
  if (lhs_.)
  switch (op_) {
    case OpEqual:
      return lhs_ == rhs_;
      break;
    case OpGreater: 
      return lhs_ < rhs_;
      break;
    case OpLess: break;
    case OpGE: break;
    case OpLE: break;
  }
}  

bool Logic::evalute(AttrVal lhs) {
  lhs_ = lhs;
  evalute();
}

} // namespace util
