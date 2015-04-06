#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>

namespace ac {
class Obj {
 public:
  enum ObjType {
    OTable,
    ORow,
    OCol,
    OCell
  };

  Obj(const std::string& tbName);
  Obj(const std::string& tbName, const int rName);
  Obj(const std::string& tbName, const std::string& cName);
  obj(const std::string& tbName, const int rName, const std::string& cName);
 
  // Get parent objects.
  std::vector<Obj> getParent();
  
  // Serialize the object to a string.
  std::string serialize();
 public:
  ObjType type_;
  std::string tbName_;
  int rName_;
  std::string cName_;
  std::string serializeStr;
}; // class Obj
} // namespace ac

#endif // OBJ_H
