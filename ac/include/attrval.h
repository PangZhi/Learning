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
    kBoolean= 0,
    kInt,
    kDouble,
    kString,
    kInvalid
  };

  AttrVal() : type_(kInvalid) {}
  AttrVal(bool boolVal) : type_(kBoolean) {value_.asBool = boolVal;}
  AttrVal(int intVal) : type_(kInt) {value_.asInt = intVal;}
  AttrVal(double dblVal) : type_(kDouble) {value_.asDouble = dblVal;}
  AttrVal(const string& strVal) : type_(kString) {value_.asString = new string(strVal);}
  AttrVal(const char* c) : type_(kString) {value_.asString = new string(c);}

  Type type() const {
    return type_;
  }

  
  bool operator== (const AttrVal& av) const {
    return ((this->type() == kInvalid && av.type() == kInvalid) 
            || (memcmp((void*)this, (void*)(&av), sizeof(AttrVal)) == 0));   
  }
 
  ac::serialization::AttrValMsg GetProto() const {
    ac::serialization::AttrValMsg msg;
    msg.set_type(type_);
     switch (type_) {
      case kBoolean:
        msg.set_value(&(value_.asBool), sizeof(bool));
        break;
      case kInt:
        msg.set_value(&(value_.asInt), sizeof(int));
        break;
      case kDouble:
        msg.set_value(&(value_.asDouble), sizeof(double));
        break;
      case kString:
        msg.set_value((void*)(value_.asString), value_.asString->size());
        break;
      default:
        // TODO: What if this is not used? We should for invalid type.
        // std::cout << "ERROR: Unknown type when serializing attrval.\n";
        std::cout << "LOG: Encounter invalid type of attrval.\n";
    }
   
    return msg;
  }

  // Use protobuf to serialize the attrval.
  std::string Serialize() const {
    ac::serialization::AttrValMsg msg;
    
    msg.set_type(type_);

    switch (type_) {
      case kBoolean:
        msg.set_value(&(value_.asBool), sizeof(bool));
        break;
      case kInt:
        msg.set_value(&(value_.asInt), sizeof(int));
        break;
      case kDouble:
        msg.set_value(&(value_.asDouble), sizeof(double));
        break;
      case kString:
        msg.set_value((void*)(value_.asString), value_.asString->size());
        break;
      default:
        // TODO: What if this is not used? We should for invalid type.
        // std::cout << "ERROR: Unknown type when serializing attrval.\n";
        std::cout << "LOG: Encounter invalid type of attrval.\n";
    }
    std::string ret;
    msg.SerializeToString(&ret);
    return ret;
  }

  
  bool GetBool() const {
    return value_.asBool;
  }

  int GetInt() const {
    return value_.asInt;
  }
  
  double GetDouble() const {
    return value_.asDouble;
  }

  std::string GetString() const {
    return *(value_.asString);
  }

  std::string GetValueString() const {
    switch (type_) {
      case kBoolean:
        return value_.asBool ? "true" : "false";
      case kInt:
        return std::to_string(value_.asInt);
      case kDouble:
        return std::to_string(value_.asDouble);
      case kString:
        return *(value_.asString);
      default:
        return "";
    }
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
    /*
      switch (attrval.type()) {
        case util::AttrVal::kBoolean:
          return std::hash<bool>()(attrval.value_.asBool);
        case util::AttrVal::kInt:
        return std::hash<int>()(attrval.value_.asInt);
        case util::AttrVal::kDouble:
        return std::hash<double>()(attrval.value_.asDouble);
        case util::AttrVal::kString:
        return std::hash<string>()(*(attrval.value_.asString));
        case util::AttrVal::kInvalid:
      }
      */
      return std::hash<string>()(attrval.Serialize());
    }
  };
}
#endif // COMMON_H
