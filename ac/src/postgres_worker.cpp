#include "db/postgres_worker.h"
#include "expression/comparison_predicate.h"
#include "expression/predicate_tree.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <unordered_map>

#include <pqxx/pqxx>

namespace db {

PostgresWorker::PostgresWorker() {
  c_ptr_ = new pqxx::connection("dbname=acdb user=postgres password=fanjing0424 hostaddr=127.0.0.1 port=5432");
  if (c_ptr_->is_open()) {
    std::cout << "LOG : Connect to " << c_ptr_->dbname() << "\n";
  } else {
    std::cout << "ERROR : Fail to connect to " << c_ptr_->dbname() << "\n";
    exit(-1);
  }
}

// TODO: Exception handle?
PostgresWorker::~PostgresWorker() {
  c_ptr_->disconnect();
}


int PostgresWorker::execute(std::string sqlcmd) {
  pqxx::work w(*c_ptr_);
  try {
    w.exec(sqlcmd);
    w.commit();
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return -1;
  }
  return 0;
}

std::vector<std::vector<util::AttrVal>> PostgresWorker::select(std::string sqlcmd) {
  pqxx::nontransaction nt(*c_ptr_);
  pqxx::result ret(nt.exec(sqlcmd));
  std::vector<std::vector<util::AttrVal>> retVec;
  
  for (pqxx::result::const_iterator iter = ret.begin();
       iter != ret.end();
       ++iter) {
        // std::cout << iter[0].as<int>();
        int size = iter->size();
        std::vector<util::AttrVal> attrVec;
        for (int i = 0; i < size; ++i) {
          // attrVec.push_back(new util::AttrVal(iter[i]));
        } 
        retVec.push_back(attrVec);
       }
  return retVec;
}

/**
bool PostgresWorker::hasValidRule(std::string sqlcmd){ 
  pqxx::nontransaction nt(*c_ptr_);
  pqxx::result ret(nt.exec(sqlcmd));

  int ruleNum = ret[0][0].as<int>();
  return (ruleNum > 0);
}
*/

bool PostgresWorker::hasValidRule(
    const std::string& drop_candidate_table,
    const std::string& generate_candidate_table,
    const std::string& select_value_independent,
    const std::vector<std::string>& select_value_dependent) const {
  pqxx::nontransaction nt(*c_ptr_);
  try {
    nt.exec(drop_candidate_table);
  } catch (const std::exception& e) {
    std::cout << "WARNING: Drop table candidate fail.\n";
  }

  try {
    nt.exec(generate_candidate_table);
  } catch (const std::exception& e) {
    std::cout << "ERROR: Generate table candidate fail.\n";
  }

  try {
    pqxx::result value_independent_ret(nt.exec(select_value_independent));
    if (value_independent_ret.size() != 0) {
      return true;
    }
  } catch (const std::exception& e) {
    std::cout << "ERROR: SELECT value independent rules fail.\n";
  }
  try {
    
    std::unordered_map<int, std::vector<ac::ComparisonPredicate>> ruleid2disjunctions;
    // Here we should consider different types.
    // Result from ruleinttb.
    static std::unordered_map<std::string, ac::Comparison> 
    op2comparison{{"=", ac::kEqual },
                  {">", ac::kGreater},
                  {"<", ac::kLess},
                  {">=", ac::kGE},
                  {"<=", ac::kLE}};
    
    
    
    for (int type_idx = 0; type_idx < 2; ++type_idx) {
      std::cout << select_value_dependent[type_idx] << "\n";
      pqxx::result value_dependent_ret(nt.exec(select_value_dependent[type_idx]));
      for (pqxx::result::const_iterator iter = value_dependent_ret.begin();
           iter != value_dependent_ret.end();
           ++iter) {
          // std::cout << iter[0].as<int>();
          int ruleid = iter["ruleid"].as<int>();
          // permission string and column string are not useful.
          std::string attrname = iter["attr"].as<std::string>();
          std::string op = iter["op"].as<std::string>();
          
          
          switch (type_idx) {
            case 0:
            {
              int value = iter["value"].as<int>();
          
              ruleid2disjunctions[ruleid].push_back( 
                ac::ComparisonPredicate(op2comparison[op], 
                                  attrname,
                                  util::AttrVal(value),
                                  ac::ComparisonPredicate::kCol2Val));       
          
              break;  
            }
            case 1:
            {
              double value = iter["value"].as<double>();
              ruleid2disjunctions[ruleid].push_back(
                ac::ComparisonPredicate(op2comparison[op],
                                    attrname,
                                    util::AttrVal(value),
                ac::ComparisonPredicate::kCol2Val));
            }
            case 2:break;
          }
      } // iter
    } // type_idx 

    std::string extra_predicates;
   
    ac::Predicate *final_pred = NULL;

    ac::DisjunctionPredicate* disjunction_predicate = new ac::DisjunctionPredicate();
    for (auto item : ruleid2disjunctions) {
      std::string conjunctions;
      ac::ConjunctionPredicate* conjunction_predicate = new ac::ConjunctionPredicate();
      for (auto literal : item.second) {
        conjunction_predicate->addPredicate(
          new ac::ComparisonPredicate(literal.comparison(),
                                      literal.col_attr(),
                                      literal.val(),
                                      ac::ComparisonPredicate::kCol2Val));
        //std::cout << literal.col_attr() << " ";
        //std::cout << GetComparisonStr(literal.comparison()) << " ";
        //std::cout << literal.val().GetValueString() << "\n";int value = iter["value"].as<int>();
        
        std::cout << literal.GetLiteralString() << "\n";
        
        // extra_predicates += literal.GetLiteralString();
        if (conjunctions == "")  {
          conjunctions += literal.GetLiteralString();
        } else {
          conjunctions = conjunctions + " AND " + literal.GetLiteralString();
        }
        
        disjunction_predicate->addPredicate(conjunction_predicate);
      }

          if (ruleid2disjunctions.size() == 1) {
            if (ruleid2disjunctions.begin()->second.size() == 1) {
              final_pred = ((ac::ConjunctionPredicate*)(disjunction_predicate->dynamic_operand_list_.front()))->dynamic_operand_list_.front();
            } else {
              final_pred = disjunction_predicate->dynamic_operand_list_.front();
            }
          } else {
            final_pred = disjunction_predicate;
          }

      std::string pb_str;

      final_pred->GetProto().SerializeToString(&pb_str);
     
      std::ofstream fout("./ac.output");

      if (!fout) {
        std::cout << "ERROR: open ac.output\n";
      }
      final_pred->GetProto().SerializeToOstream(&fout);
      fout.close();
      std::cout << pb_str << "\n";

      conjunctions = "(" + conjunctions + ")";
      if (extra_predicates == "") {
        extra_predicates = conjunctions;
      } else {
        extra_predicates = extra_predicates + " OR " + conjunctions; 
      }
    }

  std::cout << "LOG: Extra predicates is " << extra_predicates << "\n";

    // Reconstruct the disjunctions.
    return true;
  } catch (const std::exception& e) {
    std::cout << "ERROR: SELECT value dependent rules fail.\n";
  }
}

int PostgresWorker::GetRuleId(const std::string& sqlcmd) const {
  pqxx::nontransaction nt(*c_ptr_);
  pqxx::result ret(nt.exec(sqlcmd));
  if (ret[0][0].is_null()) return 0;
  return ret[0][0].as<int>();
}
} // namespace db
