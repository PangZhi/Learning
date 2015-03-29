#ifndef ATTRVAL_H 
#define ATTRVAL_H

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
  AttrVal(double dblVal) : type_(TypeDouble) {value_.asDouble = dblVal;}
  AttrVal(const string& strVal) : type_(TypeString) {value_.asString = new string(strVal);}
  AttrVal(const char* c) : type_(TypeString) {value_.asString = new string(c);}

  Type type() {
    return type_;
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

#endif // COMMON_H
