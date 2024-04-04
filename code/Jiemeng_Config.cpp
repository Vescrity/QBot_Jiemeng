#include "Jiemeng_Config.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_JsonTemplate.hpp"
void Config::init(const json &js)
{
  try
  {
    js_getval(port,js,"port");
  }
  catch(const std::exception &e)
  {
    JM_EXCEPTION("[Config]");
  }
}