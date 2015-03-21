#ifndef COMMON_H
#define COMMON_H

#include <string>
using namespace std;

namespace util {

class AttrVal {  
 public:
  enum Type {
    TypeInvalid,
    TypeBoolean,
    TypeInt,
    TypeDouble,
    TypeString
  };

  AttrVal() : type_(TypeInvalid) {}
  AttrVal(bool boolVal) : type_(TypeBoolean) {value_.asBool = boolVal;}
  AttrVal(int intVal) : type_(TypeInt) {value_.asInt = intVal;}
  AttrVal(const string& strVal) : type_(TypeString) {value_.asString = new string(strVal);}

  Type type() {
    return type_;
  }
 private:
  Type type_;
  union {
    bool asBool;
    int asInt;
    double asDouble;
    string* asString;
  } value_;
};

} // namespace util 

#endif // COMMON_H
