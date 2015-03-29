#ifndef LOGIC_H
#define LOGIC_H

namespace util {
class Logic {
 public:
  enum Op {
    OpEqual,
    OpGreater,
    OpLess,
    OpGE,
    OpLE
  };

  Logic(std::string lhs_str, Op op, AttrVal rhs)
      : lhs_str_(lhs_str), op_(op), rhs_(rhs) {}
 
  bool evaluate();
  bool evaluate(AttrVal lhs);

 public:
  Op op_;
  std::string lhs_str_;
  AttrVal lhs_;
  AttrVal rhs_;
}; // class logic
} // namespace util

#endif
