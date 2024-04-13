# QBot_Jiemeng

- 本说明适用于 v25
- 项目主要用于个人开发练习。
- 本说明用于介绍桔梦基本功能，组成，并帮助您快速上手。
- 具体的详细说明请参照进阶说明。

## 简介
桔梦(QBot_Jiemeng)是一个以 Onebot 标准的实现项目作为前端的 bot 项目。

## 提示

v25 为重构版本，对以往版本几乎没有兼容性。文档更新会有较大延迟。功能稳定的旧版本请获取 v24.13.13

## 已验证可用的平台

- Linux-x86_64
- Linux-aarch64 (Termux Proot-distro 内可构建、可使用)

## 已实现的特色应用场景举例

1. 定时消息
2. 远程指令执行
3. 可简易配置的第三方api调用
4. 用于 [Dice!](https://github.com/Dice-Developer-Team/Dice) 的牌堆调用兼容
5. 群聊检测到满足条件的信息向 bot 主或特定群聊上报消息。


## 特色

### v25 新功能：Lua 脚本支持

可以编写 Lua 脚本来一定程度上更自由的控制 Bot 了。

### 词库配置为中心

#### “一切皆消息”

将所有的事件都转换为文本消息来处理，以“收到消息——词库检索——执行应答”的流程处理所有事件。

#### 检索条件：使用正则表达式，

- 为了简化复杂的匹配逻辑用正则表达的困难与难以阅读，利用 json 数组的结构特点简化了表达

#### 较复杂的应答选取逻辑

- 提供了可调控概率的应答随机选取方法。
- 树状逻辑的应答库嵌套，即一个应答可以是若干的应答的组合，可实现更丰富的应答。
- 例如你预想的应答可分为3个部分，使用桔梦你可以很轻松的使你的随机输出可包括例如`A` `B1` `C2d` `3a` `Cb` 等等任意组合。

#### 从应答库到控制一切

结合 Lua，几乎可以控制 Bot 的所有行为。而应答库则可以为这个控制提供接口。将你需要的行为使用 Lua 实现，再将它注册到应答库中即可。



## ~~开发者的一些废话~~
~~非计算机相关专业，纯业余开发，技术稀烂，代码可读性极差，总之图一乐~~


## 构建

### Linux 编译方案：  

- 依赖：
  - cmake
  - g++
  - nlohmann-json
  - lua
  - sol2
  - boost


执行 `./build.sh` 可在 `build` 目录下构建基础的工作文件夹。

### Windows 编译方案

~~作者太菜了不理解 Windows 的奇葩编译环境故~~不提供 

## 运行依赖

- boost
- lagrange 等支持正向 WebSocket 的 OneBot 实现
- python(可选)
- python-pillow(可选) 用于txt2img功能支持
- ...



## 使用说明



### 快速开始

#### 1. 修改 `config.json`

1. 将 `Self_ID` 字段的值改为 bot qq 号
2. 在 `group_list` 中添加你想启用 bot 功能的群聊
3. 将 `admin_list` 中 100 前的数字改为 bot 主的 QQ 号

#### 2. 向 `Answer` 文件夹内添加内容

这里应当是若干个标准的 json 文件。具体格式如下：

```json
{
  "Answers": [
    
  ]
}
```
在这个中括号内部添加应答组。  

所有在这个文件夹中的json最后将会在运行时被合并到一起。

#### 应答组配置快速上手

一个应答组至少要包含三个字段：`regex` `anss` `pri`。  
其运行的逻辑是：
1. 将所有的应答组依照其 `pri` 值，从大到小排序。
2. 收到消息后，依照顺序根据 `regex` 检查是否符合要求
3. 若符合要求，依照 `anss` 来执行应答操作。

##### regex

`regex` 用于描述匹配要求。从实例来解释：

1. 要求当消息中出现 `早` 时判定为符合要求：
   > `"regex": "早"`
1. 要求当消息中需同时出现 `早` `好` 时判定为符合要求：
   > `"regex": {"and":["早","好"]}`
2. 要求当消息中出现 `早` 或 `好` 时即判定为符合要求：
   > `"regex": ["早","好"]`
3. 要求当消息中不出现 `早` 或 `好` 时即判定为符合要求：
   > `"regex": {"not":["早","好"]}`


4. 要求当消息中需要 <同时出现 `早`  `好` 时> 或 <出现 `你好` 时> 即判定为符合要求：
   > `"regex": [{"and":["早","好"]},"你好"]`
5. 要求当消息中出现 `早` 或 `好` 时，同时不出现`晚`或`午`即判定为符合要求：
   > `"regex": {"and":[["早","好"],{"not":["晚","午"]}]}`


##### anss

`anss` 用于描述应答操作。从实例来解释：

1. 只执行输出 `你好`：
   > `"anss": ["你好"]`
1. 执行输出 `你好` 或 `你不好`：
   > `"anss": ["你好","你不好"]`
1. 执行输出 `你好` 或 `你不好`, 概率为 5:1 ：
   > `"anss": [{"text":"你好","weight":5},"你不好"]`
2. 执行输出 `A` `B` `C` 与 `1` `2` 的任意组合：
   > `"anss": {"and":[["A","B","C"],["1","2"]]}`


### Deck 模块 (Dice! 牌堆)

将所有牌堆文件放入 deck 文件夹中。将如下的应答组放入应答库中，即可通过 `.draw#...` 来调用牌堆。
```json
{
      "regex": "^\\.draw#",
      "anss":[
        {"lua_call": "draw_deck"}
      ],
      "pri": 105
}
```
- 注意：并不完全兼容。需要对牌堆内部分特殊文本(如`{nick}`进行替换)


### Lua 控制

已将 Bot 控制相关的部分函数注册到了 Lua 环境中。另：
- 启动时将会从 `./luarc` `./user_luarc` 目录中加载脚本。
  - `./luarc` 默认存放由项目本身所携带的辅助脚本，不建议修改其内容。
  - `./user_luarc` 默认存放用户自定义的启动脚本。除执行顺序在 `./luarc` 后外与其无地位差别。
  - 子目录的脚本不会被加载。

### 应答内容替换

| 原           | 替换后    |
| ------------ | --------- |
| [year]       | 年        |
| [month]      | 月        |
| [yday]       | 一年第x天 |
| [mday]       | 一月第x天 |
| [wday]       | 一周第x天 |
| [hour]       | 时        |
| [min]        | 分        |
| [sec]        | 秒        |
| [group_name] | 群名      |
| [group_id]   | 群号      |
| [user_name]  | 昵称/名片 |
| [user_id]    | qq号      |





### 开始使用：
- 请确保已完成所有配置
- ~~将Jiemeng的全部文件内容放到和go-cqhttp同一文件夹下，运行start.exe即可~~
- 分别启动 OneBot 与 桔梦。





---

## 已知问题

1. 各附加功能尚未完全支持

