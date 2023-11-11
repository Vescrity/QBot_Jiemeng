#ifndef ENCRYPT
#define ENCRYPT
#include <stdio.h>
#include <time.h>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
int ENCRYPT_PASSWORD;
struct Encrypt
{
  int tm[5];
  int id[15];
  int n_id;
  string sids;
  string sid2;
  string txxt;
  int pswd;
  int tx_n[1 << 11];
  int dx[1 << 12];
  Encrypt()
  {
    memset(tm, 0, sizeof tm);
    memset(id, 0, sizeof id);
    memset(tx_n, 0, sizeof tx_n);
    memset(dx, 0, sizeof dx);
    n_id = 0;
  }
  void get_id(const string &sid, const string &id2)
  {
    long long r = 1, r2 = 1;
    for (int i = 0; i < sid.length(); i++)
    {
      int p = sid[i] - '0';
      r *= (p == 0 ? 10 : p);
    }
    for (int i = 0; i < id2.length(); i++)
    {
      int p = id2[i] - '0';
      r2 *= p;
    }
    r += r2;
    r += pswd;
    char rr[20] = {0};
    sprintf(rr, "%lld", r);
    n_id = strlen(rr);
    for (int i = 0; i < n_id; i++)
    {
      id[i] = rr[i] - '0';
    }
  }
  void get_tm()
  {
    int hr, mn;
    time_t tmpcal_ptr;
    struct tm *tmp_ptr = NULL;
    time(&tmpcal_ptr);
    tmp_ptr = localtime(&tmpcal_ptr);
    hr = tmp_ptr->tm_hour;
    mn = tmp_ptr->tm_min;
    char ss[10];
    sprintf(ss, "%02d%02d", hr, mn);
    for (int i = 0; i < strlen(ss); i++)
    {
      tm[i] = ss[i] - '0';
    }
  }
  string get_tx(const string &sid)
  {
    get_tm();
    get_id(sid, sid2);
    int it_t = 0;
    int it_s = 0;

    for (int i = 0; i < txxt.length(); i++)
    {
      tx_n[i] = int(txxt[i]) + 128;
      tx_n[i] += tm[it_t++];
      tx_n[i] += tm[it_s++];
      it_t %= 4;
      it_s %= n_id;
    } // printf("\n");
    string rt = "";
    for (int i = 0; i < txxt.length(); i++)
    {
      dx[(i << 1)] = tx_n[i] / 26 + 65;
      dx[(i << 1) | 1] = tx_n[i] % 26 + 65;
      rt = rt + char(dx[i << 1]) + char(dx[(i << 1) | 1]);
    }
    return rt;
  }
  string en_txxt;
  string de_tx()
  {
    get_tm();
    get_id(sids, sid2);
    int it_t = 0;
    int it_s = 0;
    string rt;
    for (int i = 0; i < en_txxt.length(); i++)
    {
      dx[i] = en_txxt[i] - 65;
    }
    for (int i = 0; i < (en_txxt.length() >> 1); i++)
    {
      tx_n[i] = dx[(i << 1)] * 26;
      tx_n[i] += dx[(i << 1) | 1];
      tx_n[i] -= tm[it_t++];
      tx_n[i] -= tm[it_s++] + 128;
      it_t &= 3; //it_t %= 4;
      it_s %= n_id;
      rt = rt + char(tx_n[i]);
    }
    return rt;
  }
};
string key_repost(const string &sid, const string &sid2, const int &ps)
{
  Encrypt s;
  s.txxt = ".order";
  s.pswd = ps;
  s.sid2 = sid2;
  return s.get_tx(sid);
}
string de_cr(const string &sid, const string &sid2, const string &txxt, int ps)
{
  Encrypt s;
  s.en_txxt = txxt;
  s.sids = sid;
  s.pswd = ps;
  s.sid2 = sid2;
  return (s.de_tx().c_str() + strlen(".order"));
}
#endif
