#ifndef POSTGRES_WORKER_H
#define POSTGRES_WORKER_H

#include "dbworker.h"

namespace db {

class PostgresWorker : public dbWorker {
 public:
  // TODO: Do we need these two functions?
  PostgresWorker();
  ~PostgresWorker();

  int execute(std::string sqlcmd);
  std::vector<std::vector<util::AttrVal>> select(std::string sqlcmd);

 private:
  connection* c_;
}; // DBWorker
} // namespace db

#endif // POSTGRES_WORKER_H
