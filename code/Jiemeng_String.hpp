#ifndef JIEMENG_STRING
#define JIEMENG_STRING
#include <string.h>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

/// @brief Change a number to string
/// @param num The number to change
/// @return The string
string num2str(long long num);

inline void str_replace(std::string &str, const std::string &from, const std::string &to)
{
  size_t startPos = 0;
  size_t foundPos;
  while ((foundPos = str.find(from, startPos)) != std::string::npos)
  {
    str.replace(foundPos, from.length(), to);
    startPos = foundPos + to.length();
  }
}

string url_encode(const char *msg);
inline string url_encode(const string &msg) { return url_encode(msg.c_str()); }

inline int get_st(const char *a, const char *sub_str)
{
#ifdef _WIN32
  char *d = strstr(a, sub_str);
  if (d == NULL)
    return -1;
  else
    return d - a;
#else
  char *aa = (char *)a;
  char *d = strstr(aa, sub_str);
  if (d == NULL)
    return -1;
  else
    return d - aa;
#endif
}

/// @brief 区间字符串复制 [l,r)
/// @param l 左索引
/// @param r 右索引
/// @param txt 原字符串
/// @param copy 复制至这里
inline void get_copy(int l, int r, const char *txt, char *copy)
{
  memset(copy, 0, sizeof copy);
  strncpy(copy, txt + l, size_t(r - l));
  copy[r - l] = 0;
}
inline std::string escape_string(const std::string &s)
{
  std::string result;
  result += "\"";
  for (char c : s)
  {
    switch (c)
    {
    case '\n':
      result += "\\n";
      break;
    case '\r':
      result += "\\r";
      break;
    case '\t':
      result += "\\t";
      break;
    case '\"':
      result += "\\\"";
      break;
    case '\\':
      result += "\\\\";
      break;
    default:
      result += c;
      break;
    }
  }
  result += "\"";
  return result;
}
inline char *utf8tounicode(const char *input, char *output)
{
  int len = strlen(input);
  int unicode_len = 0;
  unsigned char ch;
  unsigned char ch1, ch2, ch3, ch4;

  for (int i = 0; i < len; i++)
  {
    ch = (unsigned char)input[i];

    if (ch < 0x80)
    {
      sprintf(output + unicode_len, "\\u%04x", ch);
      unicode_len += 6;
    }
    else if (ch < 0xE0)
    {
      ch1 = ((unsigned char)ch & (0x1C)) >> 2;
      ch2 = ((unsigned char)ch & (0x03)) << 2;
      ch = ((unsigned char)input[++i]);
      ch2 |= ((unsigned char)ch & (0x30)) >> 4;
      ch3 = ((unsigned char)ch & (0x0F));
      sprintf(output + unicode_len, "\\u%02x%x%x", ch1, ch2, ch3);
      unicode_len += 6;
    }
    else if (ch < 0xF0)
    {
      ch1 = ((unsigned char)ch & (0x0F));
      ch = ((unsigned char)input[++i]);
      ch2 = ((unsigned char)ch & (0x3C)) >> 2;
      ch3 = ((unsigned char)ch & (0x03)) << 2;
      ch = ((unsigned char)input[++i]);
      ch3 |= ((unsigned char)ch & (0x30)) >> 4;
      ch4 = ((unsigned char)ch & (0x0F));
      sprintf(output + unicode_len, "\\u%x%x%x%x", ch1, ch2, ch3, ch4);
      unicode_len += 6;
    }
    else
    {
      sprintf(output + unicode_len, "\\u%04x", ch);
      unicode_len += 6;
    }
  }
  return output;
}
#endif
