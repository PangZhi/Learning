#ifndef ATTRVAL_H 
#define ATTRVAL_H

#include <cstring>
#include <functional> // hash
#include <string>
#include <iostream>

#include "attrval_msg.pb.h"

using namespace std;

namespace util {

class AttrVal {  
 public:
  enum Type {
    TypeInvalid = 0,
    TypeBoolean,
    TypeInt,
    TypeDouble,
    TypeString
  };

  AttrVal() : type_(TypeInvalid) {}
  AttrVal(bool boolVal) : type_(TypeBoolean) {value_.asBool = boolVal;}
  AttrVal(int intVal) : type_(TypeInt) {value_.asInt = intVal;}
  AttrVal(double dblVal) : type_(TypeDouble) {value_.asDouble = dblVal;}
  AttrVal(const string& strVal) : type_(TypeString) {value_.asString = new string(strVal);}
  AttrVal(const char* c) : type_(TypeString) {value_.asString = new string(c);}

  Type type() const {
    return type_;
  }

  bool operator==(const AttrVal& av) const {
    return (memcmp((void*)this, (void*)(&av), sizeof(AttrVal)) == 0);   
  }
  

  std::string serialize() {
    AttrValMsg msg;
    
    msg.set_type(type_);

    switch (type_) {
      case TypeBoolean:
        msg.set_value(&(value_.asBool), sizeof(bool));
        break;
      case TypeInt:
        msg.set_value(&(value_.asInt), sizeof(int));
        break;
      case TypeDouble:
        msg.set_value(&(value_.asDouble), sizeof(double));
        break;
      case TypeString:
        msg.set_value((void*)(value_.asString), value_.asString->size());
        break;
      default:
        std::cout << "ERROR: Unknown type when serializing attrval.\n";
    }
    std::string ret;
    msg.SerializeToString(&ret);
    return ret;
  }

 public:
  Type type_;
  union {
    bool asBool;
    int asInt;
    double asDouble;
    string* asString;
  } value_;
};

} // namespace util 


namespace std {
  template <>
  struct hash<util::AttrVal> {

    size_t operator() (const util::AttrVal& attrval) const {
      switch (attrval.type()) {
        case util::AttrVal::TypeBoolean:
          return std::hash<bool>()(attrval.value_.asBool);
          return 0;
          break;
        case util::AttrVal::TypeInt:
        return std::hash<int>()(attrval.value_.asInt);
          break;
        case util::AttrVal::TypeDouble:
        return std::hash<double>()(attrval.value_.asDouble);
          break;
        case util::AttrVal::TypeString:
        return std::hash<string>()(*(attrval.value_.asString));
          break;
      }
    }
  };
}
#endif // COMMON_H
