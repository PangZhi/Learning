#include "access_control.h"

#include "db/postgres_worker.h"

#include <iostream>
namespace ac {

AccessControl::AccessControl() {
  if (DB_FLAG == 1) {
    db_ptr_ = new db::PostgresWorker();
  } else {
    // TODO
  }
}

AccessControl::~AccessControl() {
  delete db_ptr_;
}

int AccessControl::addUser(const std::string& username,
                           const std::vector<AttrItem>& attrs) {
 
  std::string sqlcmd;
  std::string tbnames[] {"UserAttrBoolTb", "UserAttrIntTb", "UserAttrDblTb", "UserAttrStrTb"};
  for (auto item : attrs) {
        sqlcmd = "INSERT INTO " + tbnames[item.second.type_-1] + " values('"
            + username + "', '" + item.first + "', ";
        switch (item.second.type_) {
          case util::AttrVal::Type::TypeInvalid:
            break;
          case util::AttrVal::Type::TypeBoolean:
            break;
          case util::AttrVal::Type::TypeInt:
              std::cout << "int value is : " << std::to_string(item.second.value_.asInt);
              sqlcmd += std::to_string(item.second.value_.asInt); 
            break;
          case util::AttrVal::Type::TypeDouble:
              std::cout << "double value is : " << std::to_string(item.second.value_.asDouble);
              sqlcmd += std::to_string(item.second.value_.asDouble);
            break;
        }
        sqlcmd += ")";
      db_ptr_->execute(sqlcmd);
    }
  std::cout << "Call add User Successful" << std::endl;
  return 0;
}

/**
int AccessControl::addRule() {
  return 0;
}
*/

//int AccessControl::addRule(bool isAllow, Permission p, const Obj& obj, const std::string& logic) {
  // TODO: distinguish value independent logic and value dependent logic.
  // TODO: convert logic to disjunctions.
  // TODO: put the logics into rule table.
  // TODO: compute permission matrix for col level rule and table level rule.
//}

// Add another tag called is_value_dependent.
int AccessControl::addRule(bool is_allow, const Permission& p, const Obj& obj,
    const std::vector<std::vector<ComparisonPredicate> >& disjunctions) {
  // TODO: Add database function for getting rule id.
  static int ruleid = 0;
  ++ruleid;
  std::string permission_str = GetPermissionStr(p);
  std::string obj_str = obj.serialize();
  std::string sql_cmd;
  for (auto disjunction : disjunctions) {
    for (auto pred : disjunction) {
      sql_cmd = "INSERT INTO Rules VALUES('" + permission_str 
      + "','" + obj_str + "'," + std::to_string(ruleid) 
      + ",'" + pred.lop() + "','" + std::to_string(pred.comparison())
      + "','" + pred.rop().serialize() + ");";
      if (!db_ptr_->execute(sql_cmd)) {
        std::cout << "ERROR: Insert disjunctions error.\n";
      }
    }
  }
}

bool AccessControl::allow(const std::string& username, const Obj& obj, const std::string& action) {
  // go from higher level objects to lower level objects, if higher level
  // permission is granted, then low level permission should be granted
  // (assumption).

  std::vector<Obj> parents = obj.getParent();
  for (Obj& par : parents) {
    std::string objStr = par.serialize();
    std::string sqlcmd;
    sqlcmd = "SELECT COUNT(*) FROM"\
           "(SELECT DISTINCT ruleid FROM"\
           "(SELECT * FROM Rules WHERE action='" + action + "') R"\
           "LEFT OUTER JOIN"\
           "(SELECT * FROM Users WHERE userid='" + username + "') U"\
           "ON R.attrid = U.attrid"\
           "AND R.value != U.value) s";
    if (db_ptr_->hasValidRule(sqlcmd)) {
      return true;
    } 
  }

  return false;  
  //std::vectorstd<std::vector<util::AttrVal> > ret = db_ptr_->select(sqlcmd);

}

} // namespace ac
