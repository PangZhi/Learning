#ifndef COMPARISON_H
#define COMPARISON_H

#include <string>

namespace ac {
enum Comparison {
  kEqual,
  kGreater,
  kLess,
  kGE,
  kLE
}; // enum Comparison

std::string GetComparisonStr(const Comparison& comp); 
} // namespace ac
#endif
