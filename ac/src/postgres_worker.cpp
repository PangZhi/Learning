#include "db/postgres_worker.h"

#include <cstdlib>
#include <iostream>

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

bool PostgresWorker::hasValidRule(std::string sqlcmd){ 
  pqxx::nontransaction nt(*c_ptr_);
  pqxx::result ret(nt.exec(sqlcmd));

  int ruleNum = ret[0][0].as<int>();
  return (ruleNum > 0);
} 
} // namespace db
