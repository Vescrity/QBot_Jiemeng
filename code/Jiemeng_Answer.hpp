#ifndef JIEMENG_ANSWER
#define JIEMENG_ANSWER
#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Random.hpp"
#include "Jiemeng_JsonRegex.hpp"
#include "Jiemeng_Message.hpp"
#include <vector>
using namespace std;

class Answer
{
private:
  bool leaf;
  bool and_flag;
  Operation operation;
  vector<int> lvs;
  vector<Answer *> sub_ans;

public:
  Answer() { leaf = and_flag = 0; }
  ~Answer()
  {
    for (auto &i : sub_ans)
      delete i;
    sub_ans.clear();
  }
  bool is_leaf() const noexcept { return leaf; }
  bool is_and() const noexcept { return and_flag; } // TODO: 调试时若leaf时应当抛出异常，打印后应直接终止程序
  bool is_or() const noexcept { return !is_and(); }
  int rand_get() const;
  Operation_List get_list() const;
  void init(const json &js);
  void Array_init(const json &js);
};
class Answer_Group
{
private:
  bool group_check(const Message_Place &type) const;
  bool user_check(const Message_Place &type) const;
  Answer answer;
  /// @brief group 与 user 是否需要同时满足
  bool both_flag;
  int level;
  int priority;
  vector<string> grps;
  vector<string> user;
  Json_Regex regex;

public:
  bool operator<(const Answer_Group &b) const noexcept { return priority > b.priority; }
  bool type_check(const Message_Place &type) const;
  bool check(const Message &message) const;
  void init(const json &js);
  Operation_List get_list() const { return answer.get_list(); }
  Answer_Group(const json &js) { init(js); };
};
class Answer_List
{
  vector<Answer_Group *> answer_group;

public:
  int priority;
  Answer_List() { priority = 0; }
  void init(const json &js);
  void clear()
  {
    for (auto &i : answer_group)
      delete i;
    answer_group.clear();
  }
  Operation_List get_list(const Message &message) const;
  ~Answer_List() { clear(); }
};
class All_Answer
{
  vector<Answer_List *> answer_list;

public:
  void clear()
  {
    for (auto &i : answer_list)
      delete i;
    answer_list.clear();
  }
  ~All_Answer() { clear(); }
  void init(const json &);
  void main_answer_reload(const json &);
  Operation_List get_list(const Message &message) const;
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