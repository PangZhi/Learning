#include "permission.h"

namespace ac {

std::string GetPermissionStr(const Permission& p) {
  static std::string strs[]{"read", "write"};
  return strs[p];
} 
} // namespace ac
