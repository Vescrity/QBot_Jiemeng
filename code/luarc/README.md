# Jiemeng Lua

预定义了一些变量、函数、类。  
桔梦提供的 Lua api 可极大地扩展 bot 功能。  

文档对于函数原型的表示方法如下：

```
[返回类型] 函数名([参数类型] 参数名)
```

文档对于格式的描述方法：
- `(一些文本)` : 一般表示此处内容应该是什么。
  - `(#)` : 表示期望这里是配置中设置的分隔符。
- `[一些内容]` : 一般表示这部分内容是可选的。

### 用法

若需要对收到的消息做出解析并获取相关值，需要实现一个接受 `Message` 类型，返回 `string` 型的函数，并在词库中注册这个操作。当其被触发时即可调用这个函数。这里我们将这种函数称为接口函数。

## 变量

### bot

- 类型：table

预定义的函数和变量主要定义于此。

#### bot._version

- 类型：string
- 值为当前桔梦版本。

#### bot._platform

- 类型：string
- 值为当前桔梦平台。如 `Jiemeng_Linux`

#### bot._compile_time

- 类型：string
- 值为桔梦编译的时间。(实际仅为 `Jiemeng_Version.cpp` 的编译时间)

#### bot.spliter

- 类型：string
- 值为配置中指定的 spliter。默认为 `'#'`

#### bot.group_list

- 类型：string[] (type() 值为 `userdata` )
- 值为配置中指定的 group_list

## 类

### json

内部为 C++ nlohmann::json 对象。

#### 成员函数

##### new

- `[json] new()`

返回一个新的 json 对象。

##### dump

- `[string] dump()`
- `[string] dump([int] n)`

按字符串形式返回自身值。指定 `n` 可以格式化输出，`n` 为缩进大小。


### Request

#### 成员函数

##### new

- `[Request] new()`

返回一个空的 Request 对象

##### set_*

- `[void] set_url([string] url)`
- `[void] set_api([string] api)`

设置 url 与 api，发送请求时实际是直接将两者进行拼接。

- `[void] set_data([json] data)`
- `[void] set_msgs([string] msg)`

设置发送数据。
- `set_data` 参数类型为 json，并会在请求头中标记。
- `set_msgs` 直接发送设置的内容。

##### 发送方法

- `[string] Post()`
- `[string] Get()`

使用 Post/Get 方法发送 Request 类中设置的内容。

- `[json] js_post()`
- `[json] js_get()`

同上，但返回一个 json 对象。

### Message_Place

用于描述消息属性。

#### 成员

- `[string] group_id`
- `[string] group_nm`
- `[string] user_id`
- `[string] user_nm`
- `[int] level`

#### 成员函数

...

[TODO]

### Message

#### 成员

- `[string] group_id`
- `[string] group_nm`
- `[string] user_id`
- `[string] user_nm`
- `[int] level`

#### 成员函数

##### new

##### get_string

```
[string] get_string()
```

##### get_json

```
[json] get_json()
```
##### get_level

```
[int] get_level()
```

##### place

```
[Message_Place] place()
```

##### true_str

```
[string] true_str()
```
返回消息的原始文本 (移除所有非 text 段)
##### is_group

```
[bool] is_group()
```
##### is_private

```
[bool] is_private()
```
##### set_group

```
[void] set_group()
```
标记为群消息

##### set_private

```
[void] set_private()
```
标记为私聊消息
##### show

```
[void] show()
```

在终端打印消息内容


### Operaton

[TODO]

### Operaton_List

[TODO]


## 函数

### bot

#### bot.group_output
```
[bool] bot.group_output([string] group_id, [string] message)
```
###### 参数

- `group_id`: 字符串，表示群号
- `message`: 字符串，表示消息文本(CQ码字符串格式)

###### 返回

bool 型，表示是否发送成功。

#### bot.private_output
```
[bool] bot.private_output([string] user_id, [string] message)
```
###### 参数

- `user_id`: 字符串，表示用户QQ号
- `message`: 字符串，表示消息文本(CQ码字符串格式)

###### 返回

bool 型，表示是否发送成功。

#### bot._draw_deck

```
[string] bot._draw_deck([string] key)
```
牌堆抽取的基础函数。

###### 参数

- `key`: 字符串，表示牌堆名

###### 返回

`string` ，表示抽得结果。由于没有提供消息内容，对于需要从消息中获取内容才能替换的内容是无法进行替换的。例如 `[user_name]` `[Repeat]` 等。

#### bot.draw_deck

```
[string] bot._draw_deck([Message] message)
```
牌堆抽取的默认接口函数

###### 参数

- `message`: 用户消息。期望格式为 `...(#)(牌堆名)[(#)(次数)]`

###### 返回

`string` ，表示抽得结果。由于没有提供消息内容，对于需要从消息中获取内容才能替换的内容是无法进行替换的。例如 `[user_name]` `[Repeat]` 等。

#### bot.draw_list

```
[string] bot.draw_list()
```
牌堆列表。

###### 返回

所有非隐藏牌堆名，一行一个。


#### bot.deck_reload

```
[void] bot.deck_reload()
```

重新加载牌堆。当前实现为同步调用。

#### bot.deck_size

```
[int] bot.deck_size()
```

牌堆大小 = 所有可调用的牌堆项

#### bot.answer_size

```
[int] bot.answer_size()
```

应答库大小 = 基础应答库所有的应答组数量

#### bot.lua_reload

```
[string=""] bot.lua_reload()
```

调用后立即返回空字符串。当前 lua 状态不再被占用时重置当前 lua 状态。 

#### bot.ws_send

```
[json] bot.ws_send([?] data)
```

尝试将 data 格式化为 json 后对 OneBot 发送请求并等待响应，以 json 类型返回。

#### bot._ws_send

```
[void] bot._ws_send([?] data)
```
同上，但不等待返回值。调用后程序内部创建新的线程，此函数即刻返回。


#### bot.state_run
#### bot._state_run

```
[string] bot.state_run([string] code)
[void] bot._state_run([string] code)
```

使另一 lua 环境内执行指定代码块。`_state_run` 调用后不等待结果。

### bot(config)

#### bot.get_group_list

```
[string[]] bot.get_group_list()
```
获取已生效群聊列表

#### bot.get_custom_config

```
[table] bot.get_custom_config()
```

获取 Config 文件中 Custom_Config 值。

#### bot.add_group_list
#### bot.add_private_list
#### bot.add_black_list
#### bot.del_private_list
#### bot.del_group_list
#### bot.del_black_list

```
[void] bot.add/del_xx_list([string] id)
```

向指定的list中添加或删除对应的 id 。执行save前只是暂时更改。

#### bot.save_config

```
[void] bot.save_config()
```

保存配置。


### jsonlib

#### jsonlib.table2json

```
[json] jsonlib.table2json([table] t)
```

#### jsonlib.json2table

```
[table] jsonlib.json2table([json] j)
```