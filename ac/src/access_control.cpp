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

bool AccessControl::allow(const std::string& username, const std::string& object, const std::string& action) {
  std::string sqlcmd;
  sqlcmd = "SELECT count(DISTINCT ruleid) FROM"\
           "(SELECT * FROM Rules WHERE action='" + action + "') R"\
           "LEFT OUTER JOIN"\
           "(SELECT * FROM Users WHERE userid='" + username + "') U"\
           "ON R.attrid = U.attrid"\
           "AND R.value != U.value";
  
  std::vector<std::vector<util::AttrVal> > ret = db_ptr_->select(sqlcmd);
  
  return true;

}

} // namespace ac
