#ifndef POSTGRES_WORKER_H
#define POSTGRES_WORKER_H

#include "db/db_worker.h"
#include "pqxx/pqxx"
namespace db {

class PostgresWorker : public DBWorker {
 public:
  // TODO: Do we need these two functions?
  PostgresWorker();
  ~PostgresWorker();

  int execute(std::string sqlcmd);
  std::vector<std::vector<util::AttrVal>> select(std::string sqlcmd);
  bool hasValidRule(std::string sqlcmd);
  int GetRuleId(const std::string& sqlcmd) const;
private:
  pqxx::connection* c_ptr_;
}; // DBWorker
} // namespace db

#endif // POSTGRES_WORKER_H
