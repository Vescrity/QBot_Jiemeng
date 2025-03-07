#include <random>
#include "Jiemeng_Random.hpp"
#include "Jiemeng_String.hpp"
static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
default_random_engine _e(seed);

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

std::string calc_nkh(std::string Rstr)
{
  boost::regex reg("\\d*[dD]\\d+");
  boost::smatch m;
  auto ret = boost::regex_search(Rstr, m, reg);
  {
    std::string matc = Rstr;
    std::string prf, suf;

    while (ret)
    {
      prf = m.prefix();
      suf = m.suffix();
      matc = m[0];
      Lint l, r;
      if (matc[0] == 'd' || matc[0] == 'D')
      {
        l = 1;
        sscanf(matc.c_str(), "%*[dD]%lld", &r);
      }
      else
        sscanf(matc.c_str(), "%lld%*[dD]%lld", &l, &r);
      Lint rd = adb(l, r);
      matc = num2str(rd);

      matc = prf + matc + suf;

      ret = boost::regex_search(matc, m, reg);
    }
    Rstr = matc;
  }

  reg = boost::regex("\\d+[\\^]\\d+");
  // boost::smatch m;
  ret = boost::regex_search(Rstr, m, reg);
  if (ret)
  {
    std::string matc;
    std::string prf, suf;
    while (ret)
    {
      prf = m.prefix();
      suf = m.suffix();
      matc = m[0];
      Lint l, r;
      sscanf(matc.c_str(), "%lld^%lld", &l, &r);
      Lint rd = (Lint)(pow(l, r));
      matc = num2str(rd);
      matc = prf + matc + suf;
      ret = boost::regex_search(matc, m, reg);
      
    }
    Rstr = matc;
  }
  reg = boost::regex("\\d+[\\*][\\-]?\\d+");
  ret = boost::regex_search(Rstr, m, reg);
  if (ret)
  {
    std::string matc;
    std::string prf, suf;
    while (ret)
    {
      prf = m.prefix();
      suf = m.suffix();
      matc = m[0];
      Lint l, r;
      sscanf(matc.c_str(), "%lld*%lld", &l, &r);
      Lint rd = l * r;
      matc = num2str(rd);
      matc = prf + matc + suf;
      ret = boost::regex_search(matc, m, reg);
    }
    Rstr = matc;
  }
  reg = boost::regex("\\d+[\\+-][\\-]?\\d+");
  ret = boost::regex_search(Rstr, m, reg);
  if (ret)
  {
    std::string matc;
    std::string prf, suf;
    while (ret)
    {
      prf = m.prefix();
      suf = m.suffix();
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
      matc = num2str(rd);
      matc = prf + matc + suf;
      ret = boost::regex_search(matc, m, reg);
      
    }
    Rstr = matc;
  }
  return Rstr;
}

std::string calc_kh(const std::string &Rstr)
{
  boost::regex reg3(R"(\d*[dD][\d(]+)");
  boost::smatch m3;
  auto ret3 = boost::regex_search(Rstr, m3, reg3);
  if (!ret3)
    return Rstr;
  boost::regex reg(R"([\(][^\(\)]+[\)])");
  boost::regex reg2(R"([CQabcefghijlmnopqrstuvwxyz_=\$#~])");
  boost::smatch m, n;
  auto ret2 = boost::regex_search(Rstr, n, reg2);
  if (ret2)
    return Rstr;
  auto ret = boost::regex_search(Rstr, m, reg);

  std::string matc = Rstr;
  std::string prf, suf;
  while (ret)
  {
    prf = m.prefix();
    suf = m.suffix();
    matc = m[0];
    matc = matc.substr(1, matc.length() - 2);
    matc = calc_nkh(matc);
    matc = prf + matc + suf;
    ret = boost::regex_search(matc, m, reg);
  }
  return calc_nkh(matc);
}

std::string chg_Rcode(const std::string &Rstr)
{
  boost::regex reg(R"(\[[^\[]*?\])");
  boost::smatch m;
  auto ret = boost::regex_search(Rstr, m, reg);
  if (!ret)
    return Rstr;
  std::string matc;
  std::string prf, suf;

  prf = m.prefix();
  suf = m.suffix();
  matc = m[0];
  std::string tmp = matc;
  matc = matc.substr(1, matc.length() - 2);
  std::string tmp2 = matc;
  matc = calc_kh(matc);
  if (matc == tmp2)
    matc = tmp;
  matc = prf + matc + chg_Rcode(suf);

  return matc;
}
