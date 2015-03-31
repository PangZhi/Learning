#ifndef DB_H
#define DB_H

#include <string>
#include <vector>

#include "attrval.h"

namespace db {

class DBWorker {
 public:
  // TODO: Do we need these two functions?
  DBWorker();
  virtual ~DBWorker();

  virtual int execute(std::string sqlcmd) = 0;
  virtual std::vector<std::vector<util::AttrVal> > select(std::string sqlcmd) = 0;

}; // DBWorker
} // namespace db

#endif // DB_H
