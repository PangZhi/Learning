#ifndef PERMISSION_H
#define PERMISSION_H

#include <string>

namespace ac {
enum Permission {
  PRead=0,
  PWrite,
  PDeny
}; // class Permission

std::string GetPermissionStr(const Permission& p);
} // namespace ac

#endif // PERMISSION_H
