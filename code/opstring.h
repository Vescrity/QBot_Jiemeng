#ifndef OPSTRING
#define OPSTRING
#include <string.h>
#include <stdio.h>
#include <string>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

#define STR_DECODE(_str)      \
  strchg("&#91;", "[", _str); \
  strchg("&#93;", "]", _str); \
  strchg("&amp;", "&", _str);
/// @brief Change a number to string
/// @param num The number to change
/// @return The string
string num2str(long long num)
{
  char *slfid = new char[20];
  sprintf(slfid, "%lld", (num));
  return slfid;
}


void str_replace(std::string &str, const std::string &from, const std::string &to)
{
  size_t startPos = 0;
  size_t foundPos;
  while ((foundPos = str.find(from, startPos)) != std::string::npos)
  {
    str.replace(foundPos, from.length(), to);
    startPos = foundPos + to.length();
  }
}

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
inline bool pfct(const char *a, const char *b)
{
  return get_st(a, b) == 0;
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
inline void strchg(const char *oldstr, const char *newstr, char *str)
{
  char *bstr = new char[1 << 15];
  memset(bstr, 0, sizeof(bstr));
  for (int i = 0; i < strlen(str); i++)
  {
    if (!strncmp(str + i, oldstr, strlen(oldstr)))
    {
      strcat(bstr, newstr);
      i += strlen(oldstr) - 1;
    }
    else
    {
      strncat(bstr, str + i, 1);
    }
  }
  strcpy(str, bstr);
  delete[] bstr;
  return;
}
inline std::string str_strchg(const char *oldstr, const char *newstr, const char *str)
{
  std::string result = str;
  std::string oldSubstr = oldstr;
  std::string newSubstr = newstr;

  size_t pos = 0;
  while ((pos = result.find(oldSubstr, pos)) != std::string::npos)
  {
    result.replace(pos, oldSubstr.length(), newSubstr);
    pos += newSubstr.length();
  }

  return result;
}
inline void strchg_1(const char *oldstr, const char *newstr, char *str)
{
  char bstr[1 << 15];
  bool ifcpy = 0;
  memset(bstr, 0, sizeof(bstr));
  for (int i = 0; i < strlen(str); i++)
  {
    if ((!strncmp(str + i, oldstr, strlen(oldstr))) && !ifcpy)
    {
      strcat(bstr, newstr);
      i += strlen(oldstr) - 1;
      ifcpy = 1;
    }
    else
    {
      strncat(bstr, str + i, 1);
    }
  }
  strcpy(str, bstr);
  return;
}
inline std::string str_strchg_1(const char *oldstr, const char *newstr, const char *str)
{
  const int old_len = strlen(oldstr);
  const int str_len = strlen(str);
  const int new_len = strlen(newstr);
  std::string bstr;
  bool ifcpy = false;
  for (int i = 0; i < str_len; i++)
  {
    if ((!strncmp(str + i, oldstr, old_len)) && !ifcpy)
    {
      bstr.append(newstr);
      i += old_len - 1;
      ifcpy = true;
    }
    else
    {
      bstr.push_back(str[i]);
    }
  }
  return bstr;
}
static unsigned char enc_tab[] = "0123456789ABCDEF";
inline char *acl_url_encode(const char *str)
{
  int len = (int)strlen(str);
  int tmp_len = len;
  unsigned char *tmp = (unsigned char *)malloc(len + 1);
  int i, j;
  for (i = 0, j = 0; i < len; i++, j++)
  {
    tmp[j] = (unsigned char)str[i];
    if (!isalnum(tmp[j]) && strchr("_-.", tmp[j]) == NULL)
    {
      tmp_len += 3;
      tmp = (unsigned char *)realloc(tmp, tmp_len);
      tmp[j++] = '%';
      tmp[j++] = enc_tab[(unsigned char)str[i] >> 4];
      tmp[j] = enc_tab[(unsigned char)str[i] & 0x0F];
    }
  }
  tmp[j] = '\0';
  return (char *)tmp;
}
string url_encode(const char *msg)
{
  char *ff = acl_url_encode(msg);
  string rt(ff);
  free(ff);
  return rt;
}
string url_encode(const string &msg) { return url_encode(msg.c_str()); }
string file_path_chg(const char *path)
{
  string opt_path("file:///");
  char b[1 << 10] = {0};
  strcpy(b, path);
  strchg("\\", "/", b);
  // puts(b);
  opt_path = opt_path + b;
  return opt_path;
}
#include <locale>
#include <codecvt>

/*std::string UTF8ToGBK(const char *strUTF8)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::wstring wstr = conv.from_bytes(strUTF8);
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> conv_gbk(new std::locale("zh_CN.GBK"));
    return conv_gbk.to_bytes(wstr);
}

std::string GBKToUTF8(const char *strGBK)
{
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> conv_gbk(new std::locale("zh_CN.GBK"));
    std::wstring wstr = conv_gbk.from_bytes(strGBK);
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(wstr);
}*/

#ifdef _WIN32
string GBKToUTF8(const char *strGBK)
{
  int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
  wchar_t *wstr = new wchar_t[len + 1];
  memset(wstr, 0, len + 1);
  MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
  len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
  char *str = new char[len + 1];
  memset(str, 0, len + 1);
  WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
  string strTemp = str;
  if (wstr)
    delete[] wstr;
  if (str)
    delete[] str;
  return strTemp;
}

string UTF8ToGBK(const char *strUTF8)
{
  int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
  wchar_t *wszGBK = new wchar_t[len + 1];
  memset(wszGBK, 0, len * 2 + 2);
  MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
  len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
  char *szGBK = new char[len + 1];
  memset(szGBK, 0, len + 1);
  WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
  string strTemp(szGBK);
  if (wszGBK)
    delete[] wszGBK;
  if (szGBK)
    delete[] szGBK;
  return strTemp;
}
#endif
int get_16_10(const char str)
{
  if (str >= '0' && str <= '9')
    return str - '0';
  else
    return str - 'a' + 10;
}
char *utf8tounicode(const char *input, char *output)
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
char *unicode_chg(char *str)
{
  int l = 0, r = 0;
  int cnt = 0;
  char *opt = (char *)(malloc(1 << 15 * sizeof(char)));

  strchg("\\u0026#91;", "[", str);
  strchg("\\u0026#93;", "]", str);
  strchg("\\\"", "\"", str);
  while (str[l] != '\\' && l < strlen(str))
  {
    *(opt + cnt) = str[l++];
    cnt++;
  }
  while (get_st(str + l, "\\u") != -1)
  {
    l = get_st(str + l, "\\u") + 2 + l;
    *(opt + cnt) = (0xe) << 4;
    *(opt + cnt) |= get_16_10(str[l]);
    cnt++;
    l++;
    *(opt + cnt) = (0x8) << 4;
    *(opt + cnt) |= get_16_10(str[l]) << 2;
    l++;
    *(opt + cnt) |= get_16_10(str[l]) >> 2;
    cnt++;
    *(opt + cnt) = (0x8) << 4;
    *(opt + cnt) |= (get_16_10(str[l]) & 3) << 4;
    l++;
    *(opt + cnt) |= (get_16_10(str[l]));
    l++;
    cnt++;
    while (str[l] != '\\' && l < strlen(str))
    {
      *(opt + cnt) = str[l++];
      cnt++;
    }
  }
  *(opt + cnt) = 0;
  return opt;
}
string json_read(const char *msg, const char *lable)
{
  string rt;
  int l, r, r2;
  string _lable = "\"";
  bool ifstr = 0;
  _lable = _lable + lable + "\":";
  l = get_st(msg, _lable.c_str());
  if (l == -1)
    return "[NONE]";
  l += strlen(lable) + 3;
  if (msg[l] == '\"')
    ifstr = 1;
  else if (msg[l + 1] == '\"')
  {
    ifstr = 1;
    l++;
  }
  if (ifstr)
  {
    r = l++;
    do
    {
      r++;
      r = get_st(msg + r, "\"") + r;
    } while (msg[r - 1] == '\\');
  }
  else
  {
    r = get_st(msg + l, ",") + l;
    r2 = get_st(msg + l, "}") + l;
    r = (r < r2 && (r - l) != -1) ? r : r2;
  }
  for (int i = l; i < r; i++)
  {
    rt = rt + msg[i];
  }
  return rt;
}

string msg_decode(string &msg)
{
  msg = str_strchg("&#91;", "[", msg.c_str());
  msg = str_strchg("&#93;", "]", msg.c_str());
  msg = str_strchg("&#amp;", "&", msg.c_str());
  return msg;
}
std::string escape_string(const std::string &s)
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

string change_to_regex(const string &str)
{
  string sss = str;
  sss = str_strchg("\\", "\\\\", sss.c_str());
  sss = str_strchg("^", "\\^", sss.c_str());
  sss = str_strchg("$", "\\$", sss.c_str());
  sss = str_strchg("*", "\\*", sss.c_str());
  sss = str_strchg("?", "\\?", sss.c_str());
  sss = str_strchg(".", "\\.", sss.c_str());
  sss = str_strchg("(", "\\(", sss.c_str());
  sss = str_strchg(")", "\\)", sss.c_str());
  sss = str_strchg("[", "\\[", sss.c_str());
  sss = str_strchg("]", "\\]", sss.c_str());
  sss = str_strchg("{", "\\{", sss.c_str());
  sss = str_strchg("{", "\\}", sss.c_str());
  sss = str_strchg("+", "\\+", sss.c_str());
  sss = str_strchg("|", "\\|", sss.c_str());
  return sss;
}
bool hasNonUTF8(const std::string &str)
{
  for (size_t i = 0; i < str.length(); i++)
  {
    if ((str[i] & 0b10000000) == 0b10000000)
    {
      // Check whether the character is a valid UTF-8 character
      int numBytes = 0;
      if ((str[i] & 0b11100000) == 0b11000000)
      {
        numBytes = 2;
      }
      else if ((str[i] & 0b11110000) == 0b11100000)
      {
        numBytes = 3;
      }
      else if ((str[i] & 0b11111000) == 0b11110000)
      {
        numBytes = 4;
      }
      else
      {
        return true; // not a valid UTF-8 character
      }

      for (int j = 1; j < numBytes; j++)
      {
        if (i + j >= str.length() || (str[i + j] & 0b11000000) != 0b10000000)
        {
          return true; // not a valid UTF-8 character
        }
      }

      i += numBytes - 1; // skip the bytes that represent the character
    }
  }

  return false; // all characters are valid UTF-8 characters
}
#endif
