#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Answer.hpp"
namespace Jiemeng {

bool Answer_Group::check(const Message &message) const {
    if (type_check(message))
        return regex.check(message.const_str());
    return 0;
}

bool Answer_Group::type_check(const Message_Place &type) const {
    if (type.level < level)
        return 0;
    // 1000 级以上的权限无视规则
    if (type.level >= 1000)
        return 1;
    if (both_flag)
        return group_check(type) && user_check(type);
    else
        return group_check(type) || user_check(type);
}

bool Answer_Group::group_check(const Message_Place &type) const {
    // 群聊被禁用
    if (grps[0] == "0")
        return type.is_private();
    if (grps[0] == "private_true" || grps[0] == "-private_true")
        if (type.is_private())
            return 1;
    if (type.is_private())
        return 0;

    // 200 级以上的权限在允许群聊的词条中无视规则生效
    if (type.level >= 200)
        return 1;
    // 允许任意生效群聊
    if (grps[0] == "1" || (grps[0] == "private_true" && grps.size() == 1))
        return 1;
    // -1，排除后续指定群聊
    if (grps[0] == "-1" || grps[0] == "-private_true")
        return !(Vec_Find(grps, type.group_id));
    else
        return (Vec_Find(grps, type.group_id));
}
bool Answer_Group::user_check(const Message_Place &type) const {
    // 200 级以上
    if (type.level >= 200)
        return 1;
    // 允许任意有效用户
    if (user[0] == "1")
        return 1;
    if (user[0] == "0")
        return 0;
    // -1，排除后续指定用户
    if (user[0] == "-1")
        return !(Vec_Find(user, type.user_id));
    else
        return (Vec_Find(user, type.user_id));
}
} // namespace Jiemeng
