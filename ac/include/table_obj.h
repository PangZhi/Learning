#ifndef TABLE_OBJ_H
#define TABLE_OBJ_H

#include <string>
#include <vector>

namespace ac {
class TableObj {
 public:
  TableObj(const std::string& tb_name);
  TableObj(const std::string& tb_name, const std::vector<std::string>& col_names);

  std::string tb_name() const {
    return tb_name_;
  }

  std::vector<std::string> col_names() const {
    return col_names_;
  }

 private:
  std::string tb_name_;
  std::vector<std::string> col_names_;
  bool refer_all_cols_;
}; // class TableObj 

 } // namespace ac
#endif
