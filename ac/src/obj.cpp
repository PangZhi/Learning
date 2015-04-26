#include "obj.h"

namespace ac {

Obj::Obj(const std::string& tbName) : tbName_(tbName), type_(OTable) {
}

  Obj::Obj(const std::string& tbName, const int rName) : tbName_(tbName), rName_(rName) {
    type_ = ORow;
  }

  Obj::Obj(const std::string& tbName, const std::string& cName) : tbName_(tbName), cName_(cName) {
    type_ = OCol;
  }

  Obj::Obj(const std::string& tbName, const int rName, const std::string& cName) {
  }

std::vector<Obj> Obj::getParent() const {
  std::vector<Obj> ret;
  switch (type_) {
    case OTable:
      break;
    case ORow: case OCol:
      ret.push_back(Obj(tbName_));
      break;
    case OCell:
      ret.push_back(Obj(tbName_));
      ret.push_back(Obj(tbName_, rName_));
      ret.push_back(Obj(tbName_, cName_));
      break;
  }
  ret.push_back(*this);
  return ret;
}

std::string Obj::serialize() const {
  std::string ret;
  ret.append(tbName_);
  switch (type_) {
    case ORow:
      ret.append("#row#").append(std::to_string(rName_));
      break;
    case OCol:
      ret.append("#col#").append(cName_);
      break;
    case OCell:
      ret.append("#row#").append(std::to_string(rName_)).append("#col#").append(cName_);
      break;
  }
  return ret;
}

} // namespace ac
