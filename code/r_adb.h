
#ifndef R_ADB
#define R_ADB
#include <stdio.h>
#include <regex>
#include <random>
#include <chrono>
#include <time.h>
#include <string.h>
using namespace std;
auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
default_random_engine _e(seed);
#define Lint long long
Lint Rands(Lint l, Lint r)
{
  uniform_int_distribution<Lint> u(l, r);
  return u(_e);
}
Lint adb(Lint a, Lint b)
{
  Lint rt = 0;
  if (a > 1 << 23)
    return 0;
  for (Lint i = 0; i < a; i++)
    rt += Rands(1, b);
  return rt;
}

string calc_nkh(string Rstr)
{
  std::regex reg(R"(\d*[dD]\d+)");
  std::cmatch m;
  auto ret = std::regex_search(Rstr.c_str(), m, reg);
  // if(!ret)return Rstr;
  {
    string matc = Rstr;
    string prf, suf;
    prf = m.prefix();
    suf = m.suffix();

    while (ret)
    {
      matc = m[0];
      Lint l, r;
      if (matc[0] == 'd' || matc[0] == 'D')
      {
        l = 1;
        sscanf(matc.c_str(), "%*[dD]%lld", &r);
      }
      else
        sscanf(matc.c_str(), "%lld%*[dD]%lld", &l, &r);
      // printf("%lld,%lld",l,r);
      Lint rd = adb(l, r);
      char ss[13] = {0};
      sprintf(ss, "%lld", rd);
      matc = ss;

      matc = prf + matc + suf;

      ret = std::regex_search(matc.c_str(), m, reg);
      prf = m.prefix();
      suf = m.suffix();
    }
    Rstr = matc;
  }

  reg = regex(R"(\d+[\^]\d+)");
  // std::cmatch m;
  ret = std::regex_search(Rstr.c_str(), m, reg);
  if (ret)
  {
    string matc;
    string prf, suf;
    prf = m.prefix();
    suf = m.suffix();
    while (ret)
    {
      matc = m[0];
      Lint l, r;
      sscanf(matc.c_str(), "%lld^%lld", &l, &r);
      Lint rd = (Lint)(pow(l, r));
      char ss[13] = {0};
      sprintf(ss, "%lld", rd);
      matc = ss;
      matc = prf + matc + suf;
      ret = std::regex_search(matc.c_str(), m, reg);
      prf = m.prefix();
      suf = m.suffix();
    }
    Rstr = matc;
  }
  reg = regex(R"(\d+[\*][\-]?\d+)");
  ret = std::regex_search(Rstr.c_str(), m, reg);
  if (ret)
  {
    string matc;
    string prf, suf;
    prf = m.prefix();
    suf = m.suffix();
    while (ret)
    {
      matc = m[0];
      Lint l, r;
      sscanf(matc.c_str(), "%lld*%lld", &l, &r);
      Lint rd = l * r;
      char ss[13] = {0};
      sprintf(ss, "%lld", rd);
      matc = ss;
      matc = prf + matc + suf;
      ret = std::regex_search(matc.c_str(), m, reg);
      prf = m.prefix();
      suf = m.suffix();
    }
    Rstr = matc;
  }
  reg = regex(R"(\d+[\+-][\-]?\d+)");
  ret = std::regex_search(Rstr.c_str(), m, reg);
  if (ret)
  {
    string matc;
    string prf, suf;
    prf = m.prefix();
    suf = m.suffix();
    while (ret)
    {
      matc = m[0];
      Lint l, r;
      if (strstr(matc.c_str(), "+") == NULL)
      {
        sscanf(matc.c_str(), "%lld-%lld", &l, &r);
        r = -r;
      }
      else
        sscanf(matc.c_str(), "%lld+%lld", &l, &r);
      Lint rd = l + r;
      char ss[13] = {0};
      sprintf(ss, "%lld", rd);
      matc = ss;
      matc = prf + matc + suf;
      ret = std::regex_search(matc.c_str(), m, reg);
      prf = m.prefix();
      suf = m.suffix();
    }
    Rstr = matc;
  }
  return Rstr;
}
/*string R_chg(string Rstr){

  std::regex reg(R"(\d*[dD]\d+)");

  auto ret = std::regex_search(Rstr.c_str(), m, reg);
  if(!ret)return Rstr;
  string matc;
  string prf,suf;
  prf=m.prefix();
  suf=m.suffix();

  while (ret)
  {
      matc=m[0];
      int l,r;
      if(matc[0]=='d'||matc[0]=='D'){
        l=1;
        sscanf(matc.c_str(),"%*[dD]%lld",&r);
      }else
        sscanf(matc.c_str(),"%lld%*[dD]%lld",&l,&r);
      //printf("%lld,%lld",l,r);
      int rd=adb(l,r);
      char ss[13]={0};
      sprintf(ss,"%lld",rd);
      matc=ss;

      matc=prf+matc+suf;

      ret = std::regex_search(matc.c_str(), m, reg);
      prf=m.prefix();
      suf=m.suffix();
  }
  //matc=calc_kh(matc);
  matc=calc_nkh(matc);
  return matc;

}*/
string calc_kh(string Rstr)
{
  std::regex reg3(R"(\d*[dD][\d(]+)");
  std::cmatch m3;
  auto ret3 = std::regex_search(Rstr.c_str(), m3, reg3);
  if (!ret3)
    return Rstr;
  std::regex reg(R"([\(][^\(\)]+[\)])");
  std::regex reg2(R"([CQabcefghijlmnopqrstuvwxyz_=\$#~])");
  std::cmatch m, n;
  // string Rstr="4+3D6+2+5*3+2d5*10";
  auto ret2 = std::regex_search(Rstr.c_str(), n, reg2);
  if (ret2)
    return Rstr;
  auto ret = std::regex_search(Rstr.c_str(), m, reg);
  // if(!ret)return Rstr;

  string matc = Rstr;
  string prf, suf;
  prf = m.prefix();
  suf = m.suffix();

  while (ret)
  {
    matc = m[0];
    matc = matc.c_str() + 1;
    matc[matc.length() - 1] = 0;
    matc = calc_nkh(matc);
    matc = prf + matc + suf;
    ret = std::regex_search(matc.c_str(), m, reg);
    prf = m.prefix();
    suf = m.suffix();
  }
  return calc_nkh(matc);
}

string chg_Rcode(string Rstr)
{

  std::regex reg(R"([\[][^\[\]]*[\]])");
  std::cmatch m;
  auto ret = std::regex_search(Rstr.c_str(), m, reg);
  if (!ret)
    return Rstr;
  string matc;
  string prf, suf;
  prf = m.prefix();
  suf = m.suffix();
  // cout<<suf<<endl;
  if (ret)
  {
    matc = m[0];
    string tmp = matc;
    matc = matc.c_str() + 1;
    matc[matc.length() - 1] = 0;
    string tmp2 = matc;
    matc = calc_kh(matc);
    if (matc == tmp2)
      matc = tmp;
    matc = prf + matc + chg_Rcode(suf);
  }
  return matc;
}

#endif
