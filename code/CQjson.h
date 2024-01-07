#ifndef CQJSON
#define CQJSON
#include <string>
#include <nlohmann/json.hpp>
#include "opstring.h"
using namespace std;
using json = nlohmann::json;
std::string json2CQ(const json &message)
{
  std::string cqCode;
  for (const auto &item : message)
  {
    std::string cqType = item["type"];
    json cqData = item["data"];
    if (cqType == "text")
    {
      std::string text = cqData["text"];
      str_replace(text, "[", "&#91;");
      str_replace(text, "]", "&#93;");
      str_replace(text, "&", "&amp;");
      cqCode += text;
      continue;
    }
    cqCode += "[CQ:" + cqType;
    for (auto it = cqData.begin(); it != cqData.end(); ++it)
    {
      std::string key = it.key();
      std::string value = it.value();
      str_replace(value, "[", "&#91;");
      str_replace(value, "]", "&#93;");
      str_replace(value, "&", "&amp;");
      cqCode += "," + key + "=" + value;
    }
    cqCode += "]";
  }

  return cqCode;
}

json CQ2json(const string &message)
{
  json result;
  json temp;
  size_t endPos;
  size_t startPos = 0;
  string cqType;
  bool flg;
  do
  {
    json cqData;
    flg = 0;
    endPos = message.find("[CQ:", startPos);
    if (endPos == startPos)
    {
      flg = 1;
      endPos = message.find("]", startPos) + 1;
    }
    string cqCode = message.substr(startPos, endPos - startPos);
    if (flg)
    {
      size_t colonPos = cqCode.find(":");
      size_t fstPos = cqCode.find(",", colonPos);
      size_t feqPos, fedPos;
      string cqKey, value;
      cqType = cqCode.substr(colonPos + 1, fstPos - colonPos - 1);
      do
      {
        feqPos = cqCode.find("=", fstPos);
        cqKey = cqCode.substr(fstPos + 1, feqPos - fstPos - 1);
        fstPos = cqCode.find(",", feqPos);
        fedPos=fstPos;
        fedPos = (fedPos == string::npos) ? (cqCode.length() - 1) : fstPos;
        value = cqCode.substr(feqPos + 1, fedPos - feqPos - 1);
        cqData[cqKey] = value;
      } while (fstPos != string::npos);
    }
    else
    {
      cqType = "text";
      cqData["text"] = cqCode;
    }
    startPos = endPos;
    result.push_back({{"type", cqType}, {"data", cqData}});
  } while (endPos != string::npos);
  return result;
}

void raw_generate(json &message)
{
  message["raw_message"] = json2CQ(message["message"]);
}

#endif
