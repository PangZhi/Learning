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
        sqlcmd = "INSERT INTO " + tbnames[item.second.type()] + " VALUES ('"
            + username + "', '" + item.first + "', ";
        switch (item.second.type()) {
          case util::AttrVal::kInvalid:
            break;
          case util::AttrVal::kBoolean:
            break;
          case util::AttrVal::kInt:
              std::cout << "int value is : " << std::to_string(item.second.value_.asInt);
              sqlcmd += std::to_string(item.second.value_.asInt); 
            break;
          case util::AttrVal::kDouble:
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
  // static int ruleid = 0;
  // ++ruleid;
  // Get and rule id.
  int ruleid = db_ptr_->GetRuleId("SELECT MAX(ruleid)+1 FROM ruleidtb");
  if (db_ptr_->execute("INSERT INTO ruleidtb VALUES(" 
                       + std::to_string(ruleid) + ");")) {
      std::cout << "ERROR: INSERT ruleid failed.\n";
  }
  std::string permission_str = GetPermissionStr(p);
  std::string obj_str = obj.serialize();
  std::string sql_cmd;
  for (auto disjunction : disjunctions) {
    for (auto pred : disjunction) {
      // pred is ComparisonPredicate.
      static std::string tb_names[]{"rulebooltb", "ruleinttb", "ruledbltb", "rulestringtb"}; 
      switch (pred.comparison_type()) {
        case ac::ComparisonPredicate::kUser2Val: 
        case ac::ComparisonPredicate::kCol2Val:
        {
          bool value_dependent = (pred.comparison_type() 
                                  == ac::ComparisonPredicate::kUser2Val) ? false : true;
          std::string attr_str = (pred.comparison_type() 
                                  == ac::ComparisonPredicate::kUser2Val)
                                  ? pred.user_attr()
                                  : pred.col_attr();
          util::AttrVal attrval = pred.val();
          sql_cmd = "INSERT INTO " + tb_names[pred.val().type()] + " VALUES("
                    + std::to_string(ruleid) + ", '" + permission_str + "', '"
                    + obj_str + "', '" + attr_str + "', '" 
                    + GetComparisonStr(pred.comparison()) + "', "; 
          switch (attrval.type()) {
            case util::AttrVal::kBoolean:
            sql_cmd += std::to_string(attrval.GetBool());
            break;
            case util::AttrVal::kInt:
            sql_cmd += std::to_string(attrval.GetInt());
            break;
            case util::AttrVal::kDouble:
            sql_cmd += std::to_string(attrval.GetDouble());
            break;
            case util::AttrVal::kString:
            sql_cmd += "'" + attrval.GetString() + "'";
            break;
          }
          sql_cmd = sql_cmd + ", " + (value_dependent ? "true" : "false") 
                   + ", " + std::to_string(pred.comparison_type())
                   + ");"; 
          break;
        }
        case ac::ComparisonPredicate::kUser2Col: 
        {
          sql_cmd = "INSERT INTO rulestringtb VALUES("  
                  + std::to_string(ruleid) + ", '" + permission_str, + "', '"
                  + obj_str + "', '" + pred.user_attr()  + "', '"
                  + GetComparisonStr(pred.comparison()) + "', '"
                  + pred.col_attr() + std::to_string(true) 
                  + "', '" + std::to_string(pred.comparison_type())
                  + ");";
          break;
        }

      }
      
      if (db_ptr_->execute(sql_cmd)) {
        std::cout << "ERROR: Insert disjunctions error.\n";
      }
    }
  }
}

bool AccessControl::allow(const std::string& username, const Obj& obj, const Permission& p) {
  // go from higher level objects to lower level objects, if higher level
  // permission is granted, then low level permission should be granted
  // (assumption).

  std::string action = GetPermissionStr(p);
  std::vector<Obj> parents = obj.getParent();
  for (Obj& par : parents) {
    std::string objStr = par.serialize();
    // std::string sqlcmd;
    std::string op[]{"=", ">", "<", ">=", "<="};
    std::string negation_op[] = {"!=", "<=", ">=", "<", ">"};
    // TODO: Add support for more types here.
    std::string user_tb_names[]{"userattrinttb", "userattrdbltb"};
    std::string rule_tb_names[]{"ruleinttb", "ruledbltb"}; 
    
    std::string sql_cmd;
    
    for (int type_idx = 0; type_idx < 2; ++type_idx) {
      std::string select_from_diff_types;
      std::vector<std::string> select_invalid_rules;
      for (int op_idx = 0; op_idx < 2; ++op_idx) {
        std::string select_invalid_rule
          = "SELECT ruleid FROM "\
            "(SELECT * FROM " + rule_tb_names[type_idx] + " WHERE permission = '"\
            + action + "' AND op = '" + op[op_idx] + "') R "\
            "LEFT OUTER JOIN 	"\
            "(SELECT * FROM " + user_tb_names[type_idx] + " WHERE username = '"
            + username + "') U "\
            "ON R.attr = U.attrname "\
            "WHERE U.val " + negation_op[op_idx] + " R.value OR username IS NULL";
        select_invalid_rules.push_back(select_invalid_rule);
        if (op_idx == 0) {
          select_from_diff_types = select_invalid_rule;
        } else {
          select_from_diff_types = select_from_diff_types + " UNION ALL "
                                 + select_invalid_rule;
        }
      }
      if (0 == type_idx) {
        sql_cmd = select_from_diff_types;
      } else {
        sql_cmd = sql_cmd + " UNION ALL " + select_from_diff_types;
      }
    }

    sql_cmd = "SELECT COUNT(*) FROM "\
    		  "(SELECT ruleid FROM ruleidtb "\
    		  "EXCEPT "\
    		  "SELECT DISTINCT ruleid FROM ("
          + sql_cmd + ") newtb"\
          ") tmptb";
    
    //std::string select_invalid_rule = "SELECT ruleid FROM"
    //                                  "(SELECT * FROM ruleinttb)"    
    
    std::cout << "DEBUG: " << sql_cmd << std::endl;
    if (db_ptr_->hasValidRule(sql_cmd)) {
      return true;
    } 
  }

  return false;  
  //std::vectorstd<std::vector<util::AttrVal> > ret = db_ptr_->select(sqlcmd);

}

} // namespace ac
