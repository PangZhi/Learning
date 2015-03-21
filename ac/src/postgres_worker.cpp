#include "postgres_worker.h"

#include <cstdlib>
#include <iostream>

#include <pqxx/pqxx>

namespace db {

PostgresWorker::PostgresWorker() {
  c_ptr_ = new connection("dbname=acdb user=postgres password=fanjing0424 hostaddr=127.0.0.1 port=5432");
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
  work w(*c_ptr_);
  try {
    w.exec(sqlcmd);
    w.commit();
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return -1;
  }
}

std::vector<std::vector<util::AttrVal>>&& select(std::string sqlcmd) {
  nontransaction nt(*c_ptr_);
  result ret(nt.exec(sqlcmd));
  std::vector<std::vector<util::AttrVal>> retVec;
  
  for (result::const_iterator iter = ret.begin();
       iter != ret.end();
       ++iter) {
        std::cout << iter[0].as<int>();
       }
  return retVec;
}

} // namespace db
