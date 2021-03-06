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
  std::string permission_str = GetPermissionStr(p);
  std::string obj_str = obj.serialize();
  std::string sql_cmd;
  for (auto disjunction : disjunctions) {
    bool rule_value_dependent = false;
    for (auto pred : disjunction) {
      // pred is ComparisonPredicate.
      static std::string tb_names[]{"rulebooltb", "ruleinttb", "ruledbltb", "rulestringtb"}; 
      switch (pred.comparison_type()) {
        case ac::ComparisonPredicate::kUser2Val: 
        case ac::ComparisonPredicate::kCol2Val:
        {
          bool value_dependent = (pred.comparison_type() 
                                  == ac::ComparisonPredicate::kUser2Val) ? false : true;
          
          if (value_dependent) {
            rule_value_dependent = true;
          }
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

    std::string insert_ruleid = "INSERT INTO ruleidtb VALUES(" 
                        + std::to_string(ruleid++) 
                        + ", " + (rule_value_dependent ? "true" : "false")  
                        + ");";
    if (db_ptr_->execute(insert_ruleid)) {
      std::cout << "ERROR: INSERT ruleid failed.\n";
    }
 
  }
}


int AccessControl::addRule(bool is_allow, const Permission& p, const TableObj& obj,
    const std::vector<std::vector<ComparisonPredicate> >& disjunctions) {
  // TODO: Add database function for getting rule id.
  // static int ruleid = 0;
  // ++ruleid;
  // Get and rule id.
  int ruleid = db_ptr_->GetRuleId("SELECT MAX(ruleid)+1 FROM ruleidtb");
  std::string permission_str = GetPermissionStr(p);
  // std::string obj_str = obj.serialize();
  
  std::vector<std::string> columns = obj.col_names();
  std::string col_str="{";
  for (int i = 0; i < columns.size(); ++i) {
    if (0 == i) {
      col_str += "\"" + columns[i] +  "\"";
    } else {
      col_str += ",\"" + columns[i] + "\"";
    }
  }
  col_str += "}";
 
  std::string tb_str = obj.tb_name();

     
      
      
  std::string sql_cmd;
  for (auto disjunction : disjunctions) {
    bool rule_value_dependent = false;
    for (auto pred : disjunction) {
      // pred is ComparisonPredicate.
      static std::string tb_names[]{"rulebooltb", "ruleinttb", "ruledbltb", "rulestringtb"}; 
      switch (pred.comparison_type()) {
        case ac::ComparisonPredicate::kUser2Val: 
        case ac::ComparisonPredicate::kCol2Val:
        {
          bool value_dependent = (pred.comparison_type() 
                                  == ac::ComparisonPredicate::kUser2Val) ? false : true;
          
          if (value_dependent) {
            rule_value_dependent = true;
          }
          std::string attr_str = (pred.comparison_type() 
                                  == ac::ComparisonPredicate::kUser2Val)
                                  ? pred.user_attr()
                                  : pred.col_attr();
          util::AttrVal attrval = pred.val();
          sql_cmd = "INSERT INTO " + tb_names[pred.val().type()] + " VALUES("
                    + std::to_string(ruleid) + ", '" + permission_str + "', '"
                    + tb_str + "', '" + attr_str + "', '" 
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
           + ", '" + col_str + "');"; 

           break;
        }
        case ac::ComparisonPredicate::kUser2Col: 
        {
          sql_cmd = "INSERT INTO rulestringtb VALUES("  
                  + std::to_string(ruleid) + ", '" + permission_str, + "', '"
                  + tb_str + "', '" + pred.user_attr()  + "', '"
                  + GetComparisonStr(pred.comparison()) + "', '"
                  + pred.col_attr() + std::to_string(true) 
                  + "', '" + std::to_string(pred.comparison_type())
                  + ", '" + col_str + "');";
          break;
        }

      }
      
      if (db_ptr_->execute(sql_cmd)) {
        std::cout << "ERROR: Insert disjunctions error.\n";
      }
    }

    std::string insert_ruleid = "INSERT INTO ruleidtb VALUES(" 
                        + std::to_string(ruleid++) 
                        + ", " + (rule_value_dependent ? "true" : "false")  
                        + ", '" + tb_str + "','" + col_str +"');";
    if (db_ptr_->execute(insert_ruleid)) {
      std::cout << "ERROR: INSERT ruleid failed.\n";
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
    
    std::string select_candidates;
    
    for (int type_idx = 0; type_idx < 2; ++type_idx) {
      std::string select_from_diff_types;
      std::vector<std::string> select_invalid_rules;
      for (int op_idx = 0; op_idx < 2; ++op_idx) {
        std::string select_invalid_rule
          = "SELECT ruleid FROM "\
            "(SELECT * FROM " + rule_tb_names[type_idx] + " WHERE permission = '"\
            + action + "' AND op = '" + op[op_idx] + "' AND value_dependent = false) R "\
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
        select_candidates = select_from_diff_types;
      } else {
        select_candidates = select_candidates + " UNION ALL " + select_from_diff_types;
      }
    }
    
    // Drop the table candidates.
    std::string drop_candidates = "DROP TABLE candidates;";

    // Generate the table candidates.
    select_candidates = "SELECT * INTO candidates FROM "\
    		  "(SELECT ruleid FROM ruleidtb "\
    		  "EXCEPT "\
    		  "SELECT DISTINCT ruleid FROM ("
          + select_candidates + ") newtb"\
          ") tmptb";
   
    std::string check_value_independent = "SELECT * FROM candidates, ruleidtb "\
                                          "WHERE candidates.ruleid = ruleidtb.ruleid "\
                                          "AND ruleidtb.value_dependent = false;";

    
    std::vector<std::string> select_value_dependent;

    for (int type_idx = 0; type_idx < 2; ++type_idx) {
      std::string select_type_cmd = "SELECT * FROM "
                                    + rule_tb_names[type_idx] + ", candidates, ruleidtb "\
                                    "WHERE " + rule_tb_names[type_idx] + ".ruleid"\
                                    "=candidates.ruleid AND ruleidtb.ruleid = candidates.ruleid "\
                                    "AND ruleidtb.value_dependent= true AND " + rule_tb_names[type_idx]
                                    + ".value_dependent=true";
      select_value_dependent.push_back(select_type_cmd);  
    }

    //std::string select_invalid_rule = "SELECT ruleid FROM"
    //                                  "(SELECT * FROM ruleinttb)"    
    
    std::cout << "DEBUG: " << select_candidates << std::endl;
    if (db_ptr_->hasValidRule(drop_candidates,
                              select_candidates,
                              check_value_independent,
                              select_value_dependent)) {
      std::cout << "LOG: The access is allowed.\n";
      return true;
    } 
  }
  std::cout << "LOG: The access is denied.\n";
  return false;  
  //std::vectorstd<std::vector<util::AttrVal> > ret = db_ptr_->select(sqlcmd);

}

bool AccessControl::allow(const std::string& username, const TableObj& obj, const Permission& p) {
  // go from higher level objects to lower level objects, if higher level
  // permission is granted, then low level permission should be granted
  // (assumption).

  std::string action = GetPermissionStr(p);
  // std::vector<Obj> parents = obj.getParent();
  // for (Obj& par : parents) {
    //std::string objStr = par.serialize();
    // std::string sqlcmd;
    
  std::vector<std::string> columns = obj.col_names();
  std::string col_str="array[";
  for (int i = 0; i < columns.size(); ++i) {
    if (0 == i) {
      col_str += "'" + columns[i] +  "'";
    } else {
      col_str += ", '" + columns[i] + "'";
    }
  }
  col_str += "]::varchar[]";
 
  std::string tb_str = obj.tb_name();

  
  
    std::string op[]{"=", ">", "<", ">=", "<="};
    std::string negation_op[] = {"!=", "<=", ">=", "<", ">"};
    // TODO: Add support for more types here.
    std::string user_tb_names[]{"userattrinttb", "userattrdbltb"};
    std::string rule_tb_names[]{"ruleinttb", "ruledbltb"}; 
    
    std::string select_candidates;
    
    for (int type_idx = 0; type_idx < 2; ++type_idx) {
      std::string select_from_diff_types;
      std::vector<std::string> select_invalid_rules;
      for (int op_idx = 0; op_idx < 2; ++op_idx) {
        std::string select_invalid_rule
          = "SELECT ruleid FROM "\
            "(SELECT * FROM " + rule_tb_names[type_idx] + " WHERE permission = '"\
            + action + "' AND op = '" + op[op_idx] + "' AND value_dependent = false "\
            "AND tablename = '" + tb_str + "' AND columns @> " + col_str + ") R "\
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
        select_candidates = select_from_diff_types;
      } else {
        select_candidates = select_candidates + " UNION ALL " + select_from_diff_types;
      }
    }
    
    // Drop the table candidates.
    std::string drop_candidates = "DROP TABLE candidates;";

    // Generate the table candidates.
    select_candidates = "SELECT * INTO candidates FROM "\
    		  "(SELECT ruleid FROM ruleidtb "\
    		  "WHERE tablename = '" + tb_str + "' AND columns @> " + col_str 
          + " EXCEPT "\
    		  "SELECT DISTINCT ruleid FROM ("
          + select_candidates + ") newtb"\
          ") tmptb";
   
    std::string check_value_independent = "SELECT * FROM candidates, ruleidtb "\
                                          "WHERE candidates.ruleid = ruleidtb.ruleid "\
                                          "AND ruleidtb.value_dependent = false;";

    
    std::vector<std::string> select_value_dependent;

    for (int type_idx = 0; type_idx < 2; ++type_idx) {
      std::string select_type_cmd = "SELECT * FROM "
                                    + rule_tb_names[type_idx] + ", candidates, ruleidtb "\
                                    "WHERE " + rule_tb_names[type_idx] + ".ruleid"\
                                    "=candidates.ruleid AND ruleidtb.ruleid = candidates.ruleid "\
                                    "AND ruleidtb.value_dependent= true AND " + rule_tb_names[type_idx]
                                    + ".value_dependent=true";
      select_value_dependent.push_back(select_type_cmd);  
    }

    //std::string select_invalid_rule = "SELECT ruleid FROM"
    //                                  "(SELECT * FROM ruleinttb)"    
    
    std::cout << "DEBUG: " << select_candidates << std::endl;
    if (db_ptr_->hasValidRule(drop_candidates,
                              select_candidates,
                              check_value_independent,
                              select_value_dependent)) {
      std::cout << "LOG: The access is allowed.\n";
      return true;
    } 
  // }
  std::cout << "LOG: The access is denied.\n";
  return false;  
  //std::vectorstd<std::vector<util::AttrVal> > ret = db_ptr_->select(sqlcmd);

}

bool AccessControl::AddTag(const std::string& tagname,
    const std::string& tagproto) {
  std::string sql_cmd = "INSERT INTO tag VALUES('" + tagname
                      + "','" + tagproto + "')";
  if (db_ptr_->execute(sql_cmd)) {
    std::cout << "ERROR: Insert tag fail.\n";
    return false;
  }
  return true;
}

bool AccessControl::RemoveTag(const std::string& tagname) {
  std::string sql_cmd = "DELETE FROM tag WHERE tagname = '" + tagname + "'";
  if (db_ptr_->execute(sql_cmd)) {
    std::cout << "ERROR: Remove tag fail.\n";
    return false;
  }
  return true;
}

} // namespace ac
