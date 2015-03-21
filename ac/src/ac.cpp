#include "access_control.h"

#include "postgres_worker.h"

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
  String::string[] tbnames {"UserAttrBoolTb", "UserAttrIntTb", "UserAttrDblTb", "UserAttrStrTb"};
  for (auto item : attrs) {
        sqlcmd = "INSERT INTO" + tbnames[item.second.type_] + " values("
            + username + ", " + item.first + ", "
            + item.second.val_;
      db_ptr_->execute(sqlcmd);
    }
  }
}

int AccessControl::addRule() {
  return 0;
}

bool AccessControl::allow(const std::string& username, const std::string& object,
    Permission p, const std::string& logic) {
  return true;
}

} // namespace ac
