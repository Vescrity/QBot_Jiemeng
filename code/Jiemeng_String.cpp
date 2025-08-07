
#include <string.h>
#include <stdio.h>
#include <string>
using namespace std;
string num2str(long long num)
{
  return to_string(num);
}

static unsigned char enc_tab[] = "0123456789ABCDEF";

char *acl_url_encode(const char *str)
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
