#ifndef JIEMENG_STRING
#define JIEMENG_STRING
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

inline void str_replace(std::string &str, const std::string &from,
                        const std::string &to) {
    size_t startPos = 0;
    size_t foundPos;
    while ((foundPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(foundPos, from.length(), to);
        startPos = foundPos + to.length();
    }
}
namespace Jiemeng {
inline std::string string2CQ(const std::string &str) {
    string text = str;
    str_replace(text, "&", "&amp;");
    str_replace(text, "[", "&#91;");
    str_replace(text, "]", "&#93;");
    return text;
}
} // namespace Jiemeng
/*string url_encode(const char *msg);
inline string url_encode(const string &msg) {
    return url_encode(msg.c_str());
}
std::string url_encode(std::string_view input);
*/
inline int get_st(const char *a, const char *sub_str) {
#ifdef _WIN32
    char *d = strstr(a, sub_str);
    if (d == NULL)
        return -1;
    else
        return d - a;
#else
    char *aa = (char *)a;
    const char *d = strstr(aa, sub_str);
    if (d == NULL)
        return -1;
    else
        return d - aa;
#endif
}

inline std::string escape_string(const std::string &s) {
    std::string result;
    result += "\"";
    for (char c : s) {
        switch (c) {
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
inline char *utf8tounicode(const char *input, char *output) {
    int len = strlen(input);
    int unicode_len = 0;
    unsigned char ch;
    unsigned char ch1, ch2, ch3, ch4;

    for (int i = 0; i < len; i++) {
        ch = (unsigned char)input[i];

        if (ch < 0x80) {
            sprintf(output + unicode_len, "\\u%04x", ch);
            unicode_len += 6;
        } else if (ch < 0xE0) {
            ch1 = ((unsigned char)ch & (0x1C)) >> 2;
            ch2 = ((unsigned char)ch & (0x03)) << 2;
            ch = ((unsigned char)input[++i]);
            ch2 |= ((unsigned char)ch & (0x30)) >> 4;
            ch3 = ((unsigned char)ch & (0x0F));
            sprintf(output + unicode_len, "\\u%02x%x%x", ch1, ch2, ch3);
            unicode_len += 6;
        } else if (ch < 0xF0) {
            ch1 = ((unsigned char)ch & (0x0F));
            ch = ((unsigned char)input[++i]);
            ch2 = ((unsigned char)ch & (0x3C)) >> 2;
            ch3 = ((unsigned char)ch & (0x03)) << 2;
            ch = ((unsigned char)input[++i]);
            ch3 |= ((unsigned char)ch & (0x30)) >> 4;
            ch4 = ((unsigned char)ch & (0x0F));
            sprintf(output + unicode_len, "\\u%x%x%x%x", ch1, ch2, ch3, ch4);
            unicode_len += 6;
        } else {
            sprintf(output + unicode_len, "\\u%04x", ch);
            unicode_len += 6;
        }
    }
    return output;
}
#endif
