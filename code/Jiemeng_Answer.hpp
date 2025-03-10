#ifndef JIEMENG_ANSWER
#define JIEMENG_ANSWER
#include "Jiemeng_JsonRegex.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng_Operation.hpp"
#include <vector>
using namespace std;
namespace Jiemeng {
class Answer {
  private:
    bool leaf;
    bool and_flag;
    Operation operation;
    vector<int> lvs;
    vector<Answer *> sub_ans;
    /// @brief 根据规则随机取应答
    /// @return 应答索引
    int rand_get() const;

  public:
    Answer() { leaf = and_flag = 0; }
    ~Answer() {
        for (auto &i : sub_ans)
            delete i;
        sub_ans.clear();
    }
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
    Answer_Group(const json &js) { init(js); };
};
class Answer_List {
    vector<Answer_Group *> answer_group;

  public:
    int priority = 0;
    auto size() { return answer_group.size(); }
    void init(const json &js);
    void clear() {
        for (auto &i : answer_group)
            delete i;
        answer_group.clear();
    }
    Operation_List get_list(const Message &message) const;
    ~Answer_List() { clear(); }
};
class All_Answer {
    vector<Answer_List *> pre_answer_list;
    vector<Answer_List *> suf_answer_list;

  public:
    Answer_List *main_answer;
    void clear() {
        auto c = [this](vector<Answer_List *> &x) {
            for (auto &i : x)
                delete i;
            x.clear();
        };
        c(pre_answer_list);
        if (main_answer != nullptr)
            delete main_answer;
        c(suf_answer_list);
    }
    All_Answer() { main_answer = nullptr; }
    ~All_Answer() { clear(); }
    void init(const json &);
    void main_answer_reload(const json &);
    Operation_List get_list(const Message &message) const;
};
} // namespace Jiemeng

#endif
