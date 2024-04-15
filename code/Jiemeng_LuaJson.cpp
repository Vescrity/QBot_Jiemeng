#include <nlohmann/json.hpp>
#include "Jiemeng_Lua.hpp"
#include "Jiemeng.hpp"
using json = nlohmann::json;
nlohmann::json lua_table_to_json(sol::object lua_value)
{
  if (lua_value.is<bool>())
  {
    return lua_value.as<bool>();
  }
  else if (lua_value.is<int>())
  {
    return lua_value.as<int>();
  }
  else if (lua_value.is<float>() || lua_value.is<double>())
  {
    return lua_value.as<double>();
  }
  else if (lua_value.is<std::string>())
  {
    return lua_value.as<std::string>();
  }
  else if (lua_value.is<sol::table>())
  {
    auto table = lua_value.as<sol::table>();
    auto first_element = *table.begin();
    if (first_element.first.is<int>())
    {
      nlohmann::json json_array = nlohmann::json::array();
      for (const auto &pair : table)
      {
        json_array.push_back(lua_table_to_json(pair.second));
      }
      return json_array;
    }
    else
    {
      nlohmann::json json_object = json::object();
      for (const auto &pair : table)
      {
        json_object[pair.first.as<std::string>()] = lua_table_to_json(pair.second);
      }
      return json_object;
    }
  }
  else
  {
    return "<Not Support Type>"s;
  }
}
sol::object json_to_lua_table(const nlohmann::json &j, sol::state &lua)
{
  if (j.is_boolean())
  {
    return sol::make_object(lua, j.get<bool>());
  }
  else if (j.is_number())
  {
    if (j.is_number_integer())
      return sol::make_object(lua, j.get<int>());
    return sol::make_object(lua, j.get<double>());
  }
  else if (j.is_string())
  {
    return sol::make_object(lua, j.get<std::string>());
  }
  else if (j.is_array())
  {
    // 创建一个新的数组（Lua表）
    sol::table lua_table = lua.create_table();
    int index = 0;
    for (const auto &item : j)
    {
      lua_table[++index] = json_to_lua_table(item, lua);
    }
    return lua_table;
  }
  else if (j.is_object())
  {
    // 创建一个新的Lua表
    sol::table lua_table = lua.create_table();
    for (auto &[key, value] : j.items())
    {
      lua_table[key] = json_to_lua_table(value, lua);
    }
    return lua_table;
  }
  else
  { // null 或其他类型
    return sol::make_object(lua, sol::nil);
  }
}
json parse_to_json(const sol::object &obj)
{
  if (obj.is<sol::table>())
  {
    sol::table tb = obj.as<sol::table>();
    return lua_table_to_json(tb);
  }
  else if (obj.is<std::string>())
  {
    const std::string &str = obj.as<std::string>();
    try
    {
      return json::parse(str);
    }
    catch (const json::parse_error &e)
    {
      // For the purpose of this example, we'll just rethrow it
      throw;
    }
  }
  else if (obj.is<json>())
  {
    return obj.as<json>();
  }
  else
  {
    throw std::invalid_argument("Unsupported type for JSON conversion");
  }
}