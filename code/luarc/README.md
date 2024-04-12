# Jiemeng Lua

预定义了一些函数与变量。

## 函数

#### bot.group_output

[bool] bot.group_output([string] group_id, [string] message)

###### 参数

- `group_id`: 字符串，表示群号
- `message`: 字符串，表示消息文本(CQ码字符串格式)

###### 返回

bool 型，表示是否发送成功。

#### bot.private_output

[bool] bot.private_output([string] user_id, [string] message)

###### 参数

- `user_id`: 字符串，表示用户QQ号
- `message`: 字符串，表示消息文本(CQ码字符串格式)

###### 返回

bool 型，表示是否发送成功。