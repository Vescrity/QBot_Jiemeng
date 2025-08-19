#include "Jiemeng_JsonRegex.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_String.hpp"
#include <boost/regex.hpp>
#include <string>
#include <clocale>

std::wstring string_to_wstring(const std::string& str) {
    if (str.empty()) return L"";

    // 获取当前环境变量的 locale（如 "en_US.UTF-8" 或 "C.UTF-8"）
    const char* locale = std::setlocale(LC_ALL, "");
    if (!locale) {
        throw std::runtime_error("Failed to set locale from environment");
    }

    // 检查当前 locale 是否支持 UTF-8（避免非 UTF-8 环境导致乱码）
    if (std::string(locale).find("UTF-8") == std::string::npos &&
        std::string(locale).find("utf8") == std::string::npos) {
        throw std::runtime_error("Current locale is not UTF-8");
    }

    // 转换
    std::size_t len = std::mbstowcs(nullptr, str.c_str(), 0);
    if (len == static_cast<std::size_t>(-1)) {
        throw std::runtime_error("Invalid multibyte string");
    }

    std::vector<wchar_t> buf(len + 1);
    std::mbstowcs(buf.data(), str.c_str(), str.size());
    return std::wstring(buf.data(), len);
}
/*std::wstring string_to_wstring(const std::string &str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> wstring_convert;
  return wstring_convert.from_bytes(str);
}*/
bool reg_check(const string &msg, const json &regs);
inline bool reg_str_check(const string &msg, const std::string &rstr)
{
  string m = msg;
  str_replace(m, "\n", "|");
  auto wrstr = string_to_wstring(rstr);
  auto wmsg = string_to_wstring(m);
  boost::wregex reg(wrstr);
  return boost::regex_search(wmsg, reg);
}
bool reg_and_check(const string &msg, const json &regs)
{
  if (regs.is_array())
  {
    int n = regs.size();
    for (int i = 0; i < n; i++)
    {
      if (!reg_check(msg, regs[i]))
        return 0;
    }
    return 1;
  }
  else
    return reg_check(msg, regs);
}
bool reg_or_check(const string &msg, const json &regs)
{
  if (regs.is_array())
  {
    int n = regs.size();
    for (int i = 0; i < n; i++)
    {
      if (reg_check(msg, regs[i]))
        return 1;
    }
    return 0;
  }
  else
    return reg_check(msg, regs);
}

bool reg_check(const string &msg, const json &regs)
{
  try
  {
    if (regs.is_string())
      return reg_str_check(msg, regs);
    if (regs.is_array())
      return reg_or_check(msg, regs);
    if (regs.is_object())
    {
      if (regs.contains("and"))
        return reg_and_check(msg, regs["and"]);
      if (regs.contains("or"))
        return reg_or_check(msg, regs["or"]);
      if (regs.contains("not"))
        return !reg_check(msg, regs["not"]);
    }
    throw invalid_argument("无效的 regex 值。");
  }
  catch (const std::exception &e)
  {
    JM_EXCEPTION("[Regex]")
    error_print("The json reg is: ");
    error_print(regs.dump().c_str());
    return 0;
  }
}

bool Json_Regex::check(const string &msg) const { return reg_check(msg, regex); }
