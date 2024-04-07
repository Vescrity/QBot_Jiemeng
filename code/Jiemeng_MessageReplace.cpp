#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng_Time.hpp"
#include "Jiemeng_Random.hpp"

void message_replace(string &str, Message_Place &place)
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
  str_replace(str, "[group_name]", place.group_nm);
  str_replace(str, "[group_id]", place.group_id);
  str_replace(str, "[user_name]", place.user_nm);
  str_replace(str, "[user_id]", place.user_id);
}
