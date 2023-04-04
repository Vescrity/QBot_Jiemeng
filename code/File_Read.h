#ifndef FILE_READ
#define FILE_READ
#include <algorithm>
#include <fstream>
#include "json.hpp"
#include "Jiemeng_Ans.h"
#include "Jiemeng_Config.h"
void Config_File_Read()
{
  ifstream fread("config.json");
  configs.init(json::parse(fread));
}
void Ans_Read()
{
  /*ifstream fread(configs.ANS_FILENAME);*/
  ifstream in(configs.ANS_FILENAME, ios::in);
  istreambuf_iterator<char> beg(in), end;
  string strdata(beg, end);
  in.close();
  strdata = string_format_with_json("{~", "}", configs.custom_config, strdata);
  // answers.init(json::parse(fread));
  answers.init(json::parse(strdata));
}
#endif
