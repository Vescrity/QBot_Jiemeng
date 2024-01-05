#ifndef JIEMENG_OPER
#define JIEMENG_OPER
#include <string>
#include <nlohmann/json.hpp>
#include "Message_Type.h"
using json = nlohmann::json;
using namespace std;
class Answer_Oper
{
public:
  enum class Oper
  {
    output,
    order
  } oper;
  string str;
  bool ifjs;
  bool msgread;
  json js;
  Answer_Oper() { msgread = ifjs = 0; }
};
#endif