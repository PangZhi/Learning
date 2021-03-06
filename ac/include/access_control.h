#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#include "attrval.h"
#include "db/db_worker.h"
#include "permission.h"
#include "obj.h"
#include "table_obj.h"
#include "expression/comparison_predicate.h"

#include <vector>
#include <string>
#include <utility>

#define DB_FLAG 1

// TODO: Use other structure for object, logic.
namespace ac {
typedef std::pair<std::string, util::AttrVal> AttrItem;


class AccessControl {
 public:
  AccessControl();
  virtual ~AccessControl();

  int addUser(const std::string& username, const std::vector<AttrItem>& attrs);
  /**
  @brief currently support equality.
  **/
  // int addRule(bool isAllow, Permission p, const Obj& obj, 
  //            const std::string& logic);
  int addRule(bool is_allow, const Permission& p, const Obj& obj,
              const std::vector<std::vector<ComparisonPredicate> >& disjunctions);

  int addRule(bool is_allow, const Permission& p, const TableObj& obj,
              const std::vector<std::vector<ComparisonPredicate> >& disjunctions);
  
  bool allow(const std::string& username, const Obj& object,
             const Permission& p);

  bool allow(const std::string& username, const TableObj& object,
             const Permission& p);

  bool AddTag(const std::string& tagname, const std::string& tagproto);
  std::string GetTag(const std::string& tagname);

  bool RemoveTag(const std::string& tagname);
private:
  db::DBWorker* db_ptr_;

}; // AccessControl

} // namespace ac


#endif // AC_H
