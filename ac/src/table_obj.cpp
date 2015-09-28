#include "table_obj.h"

namespace ac {

TableObj::TableObj(const std::string& tb_name) 
    : tb_name_(tb_name),
      refer_all_cols_(true) {}

TableObj::TableObj(const std::string& tb_name,
         const std::vector<std::string>& col_names)
    : tb_name_(tb_name),
      col_names_(col_names),
      refer_all_cols_(false) {}

} // namespace ac
