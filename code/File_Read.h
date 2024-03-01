#ifndef FILE_READ
#define FILE_READ
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Jiemeng_Ans.h"
#include "Jiemeng_Config.h"
#include "Jiemeng_Exception.h"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>

using json = nlohmann::json;
namespace fs = std::filesystem;
/// @brief Read the config from `config.json`
void Config_File_Read()
{
  ifstream fread("config.json");
  configs.init(json::parse(fread));
}

json ans_merge(const std::string &folderPath)
{
  json mergedJson;
  std::vector<json> answerArrays;
  for (const auto &entry : fs::directory_iterator(folderPath))
  {
    if (entry.path().extension() == ".json")
    {
      std::ifstream file(entry.path());
      if (file.is_open())
      {
        json fileJson;
        file >> fileJson;
        if (fileJson.contains("Answers") && fileJson["Answers"].is_array())
        {
          answerArrays.push_back(fileJson["Answers"]);
        }
        file.close();
      }
      else
      {
        #ifndef _WIN32
        throw runtime_error("Failed to open file: "s + string(entry.path()));
        #else
        throw runtime_error("Failed to open file: "s);
        #endif
      }
    }
  }

  mergedJson["Answers"] = json::array();
  for (const auto &answers : answerArrays)
  {
    mergedJson["Answers"].insert(mergedJson["Answers"].end(), answers.begin(), answers.end());
  }
  //cerr << mergedJson << endl;
  return mergedJson;
}

/// @brief Read the answer file
void Ans_Read()
{
  try
  {
    std::ostringstream oss;
    oss << ans_merge("Answer");
    string strdata = oss.str();
    //cerr << strdata << endl;
    strdata = string_format_with_json("{~", "}", configs.custom_config, strdata);
    answers.init(json::parse(strdata));
  }
  catch (std::exception &e)
  {
    JM_EXCEPTION("[Ans_Read]")
  }
}
#endif
