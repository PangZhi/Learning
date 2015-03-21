#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#include "attrval.h"
#include "db_worker.h"

#include <vector>
#include <string>
#include <pair>

#define DB_FLAG 1

// TODO: Use other structure for object, logic.
namespace ac {

typedef std::pair<std::string, util::AttrVal> AttrItem;

class AccessControl {
 public:
  AccessControl();
  virtual ~AccessControl();

  int addUser(const std::string& username, const std::vector<AttrItem>& attrs);
  int addRule(bool isAllow, Permission p, const std::string& object, 
              const std::string& logic);

  bool allow(const std::string& username, const std::string& object,
             Permission p, const std::string& logic);
 private:
  db::DBWorker db_ptr_;

}; // AccessControl

} // namespace ac


#endif // AC_H
