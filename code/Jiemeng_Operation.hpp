#ifndef JIEMENG_OPERATION
#define JIEMENG_OPERATION
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using string = std::string;
class Message;
class Jiemeng;
class Operation
{
public:
  enum class Type
  {
    message,
    order, // json order
    lua_call,
    lua_shell,
    lua_once,
    call_state,
    ignore,
    draw_deck,
    clear
  } type;
  bool is_json;
  json data;
  string str;
  Operation();
  void set_type(const string &);
  class Clear : std::exception
  {
  };
};

class Operation_List
{

public:
  std::vector<Operation> list;
  void upgrade(Message &, Jiemeng *);
  void push_back(const Operation &b) { *this += b; };
  Operation_List &operator+=(const Operation_List &b)
  {
    list.insert(list.end(), b.list.begin(), b.list.end());
    return *this;
  }
  Operation_List &operator+=(const Operation &b)
  {
    list.push_back(b);
    return *this;
  }

  Operation_List operator+(const Operation_List &b) const
  {
    Operation_List result = *this;
    result += b;
    return result;
  }
  Operation_List operator+(const Operation &b) const
  {
    Operation_List result = *this;
    result += b;
    return result;
  }
};
/*

- Operator_List get_oper  : 传入消息，经过应答队列处理返回操作列
    原始操作列：
      1. 词库应答词条，未经转化处理
      2. 指令操作：指令名与参数列
        a. 不携带json的指令使用 lua 函数调用



    处理操作列：
      1. 全部转换为输出操作、睡眠操作、变量操作



*/
#endif