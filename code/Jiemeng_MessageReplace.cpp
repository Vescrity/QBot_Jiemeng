#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng_Time.hpp"
#include "Jiemeng_Random.hpp"
#include "Jiemeng_String.hpp"
namespace Jiemeng{
void message_replace(string &str, const Message &message)
{
  str = chg_Rcode(str);
  Time_Class tm;
  str_replace(str, "[year]", tm.get_year());
  str_replace(str, "[month]", tm.get_month());
  str_replace(str, "[yday]", tm.get_yearday());
  str_replace(str, "[mday]", tm.get_monthday());
  str_replace(str, "[wday]", tm.get_weekday());
  str_replace(str, "[hour]", tm.get_hour());
  str_replace(str, "[min]", tm.get_min());
  str_replace(str, "[sec]", tm.get_sec());
  str_replace(str, "[group_name]", message.group_nm);
  str_replace(str, "[group_id]", message.group_id);
  str_replace(str, "[user_name]", message.user_nm);
  str_replace(str, "[user_id]", message.user_id);
  str_replace(str, "[Repeat]", message.const_str());
}
}
