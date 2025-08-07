#ifndef JIEMENG_ANSWER
#define JIEMENG_ANSWER
#include "Jiemeng_JsonRegex.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng_Operation.hpp"
#include <memory>
#include <vector>
using namespace std;
namespace Jiemeng {
class Answer {
  private:
    bool leaf = false;
    bool and_flag = false;
    Operation operation;
    vector<int> lvs;
    vector<unique_ptr<Answer>> sub_ans;
    /// @brief 根据规则随机取应答
    /// @return 应答索引
    int rand_get() const;

  public:
    bool is_leaf() const noexcept { return leaf; }
    bool is_and() const noexcept { return and_flag; }
    bool is_or() const noexcept { return !is_and(); }
    Operation_List get_list() const;
    void init(const json &js);
    void Array_init(const json &js);
};
class Answer_Group {
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
    bool operator<(const Answer_Group &b) const noexcept {
        return priority > b.priority;
    }
    bool type_check(const Message_Place &type) const;
    bool check(const Message &message) const;
    void init(const json &js);
    Operation_List get_list() const { return answer.get_list(); }
    explicit Answer_Group(const json &js) { init(js); };
};
class Answer_List {
    vector<unique_ptr<Answer_Group>> answer_group;

  public:
    int priority = 0;
    auto size() { return answer_group.size(); }
    void init(const json &js);
    void clear() { answer_group.clear(); }
    Operation_List get_list(const Message &message) const;
};
class All_Answer {
    vector<unique_ptr<Answer_List>> pre_answer_list;
    vector<unique_ptr<Answer_List>> suf_answer_list;

  public:
    unique_ptr<Answer_List> main_answer = nullptr;
    void clear() {
        pre_answer_list.clear();
        suf_answer_list.clear();
    }
    void init(const json &);
    void main_answer_reload(const json &);
    Operation_List get_list(const Message &message) const;
};
} // namespace Jiemeng

#endif
