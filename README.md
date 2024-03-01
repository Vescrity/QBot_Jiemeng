# QBot_Jiemeng
使用 lagrange 作为后端，使用C++开发的QQ聊天机器人程序

- 本说明适用于v24.10，正在做迁移至lagrange的适配。故功能不全。
- 项目主要用于个人开发练习。
- 本说明用于介绍桔梦基本功能，组成，并帮助您快速上手。
- 具体的详细说明请参照进阶说明。

## 简介
桔梦(QBot_Jiemeng)是一个使用 lagrange 来达到QQ聊天功能的机器人程序。

## 已实现的特色应用场景举例

1. 定时消息
2. 远程指令执行
3. 可简易配置的第三方api调用
4. 用于 [Dice!](https://github.com/Dice-Developer-Team/Dice) 的牌堆调用兼容
5. 群聊检测到满足条件的信息向 bot 主或特定群聊上报消息。


## 特色

### 词库配置为中心

#### “一切皆消息”

将所有的事件都转换为文本消息来处理，以“收到消息——词库检索——执行应答”的流程处理所有事件。

#### 检索条件：使用正则表达式，

- 为了简化复杂的匹配逻辑用正则表达的困难与难以阅读，利用 json 数组的结构特点简化了表达

#### 较复杂的应答选取逻辑

- 提供了可调控概率的应答随机选取方法。
- 树状逻辑的应答库嵌套，即一个应答可以是若干的应答的组合，可实现更丰富的应答。
- 例如你预想的应答可分为3个部分，使用桔梦你可以很轻松的使你的随机输出可包括例如`A` `B1` `C2d` `3a` `Cb` 等等任意组合。

#### 一切操作皆可为应答

不论是发送消息，还是执行什么指令，都可以在应答里实现。简单的如执行shell指令，复杂的你甚至可以调用 api ,并根据 json 结果定制输出。



## ~~开发者的一些废话~~
~~非计算机相关专业，纯业余开发，技术稀烂，代码可读性极差，总之图一乐~~


## 构建

### Linux 编译方案：  
环境条件：确保安装boost库，curl 库，nlohmann json库，以及安装了 g++。(测试了12/13版本)   
例如 `Arch Linux` 下：
```shell
pacman -S boost nlohmann-json
# curl gcc 一般已预装，故不列出。
```
执行`./build.sh`可在`build`目录下构建基础的工作文件夹。

### Windows 编译方案

~~作者太菜了不理解 Windows 的奇葩编译环境故~~不提供于 Windows 环境下编译 Windows 版的方案。  

请于 Linux 环境下使用交叉编译工具。

```shell
# 于项目根目录执行：
x86_64-w64-mingw32-g++ code/Jiemeng.cpp -lws2_32 -o ./build/Jiemeng.exe  --static --std=c++20
```

请确保已安装编译工具，并安装好库。包括：nolhmann json以及boost库。  

使用到curl库相关功能的~~由于作者太菜了所以~~已被禁用。故无需安装 curl 库。

## 运行依赖

- boost
- lagrange
- python(可选)
- python-pillow(可选) 用于txt2img功能支持
- ...



## 使用说明


### 下载

1. 下载新版桔梦release
2. ~~按release中说明下载并配置[go-cqhttp](https://github.com/Mrs4s/go-cqhttp)~~ 配置安装 lagrange

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
   > `"regex": [["早","好"]]`
1. 要求当消息中出现 `早` 或 `好` 时即判定为符合要求：
   > `"regex": ["早","好"]`
1. 要求当消息中不出现 `早` 或 `好` 时即判定为符合要求：
   > `"regex": [{"not":["早","好"]}]`
   > 最外层中括号或许可以去掉。

2. 要求当消息中需要 <同时出现 `早`  `好` 时> 或 <出现 `你好` 时> 即判定为符合要求：
   > `"regex": [["早","好"],"你好"]`
1. 要求当消息中出现 `早` 或 `好` 时，同时不出现`晚`或`午`即判定为符合要求：
   > `"regex": [[["早","好"],{"not":["晚","午"]}]]`


##### anss

`anss` 用于描述应答操作。从实例来解释：

1. 只执行输出 `你好`：
   > `"anss": ["你好"]`
1. 执行输出 `你好` 或 `你不好`：
   > `"anss": ["你好","你不好"]`
1. 执行输出 `你好` 或 `你不好`, 概率为 5:1 ：
   > `"anss": [{"text":"你好","weight":5},"你不好"]`
2. 执行输出 `A` `B` `C` 与 `1` `2` 的任意组合：
   > `"anss": [[["A","B","C"],["1","2"]]]`


### Deck 模块 (Dice! 牌堆)

将所有牌堆文件放入 deck 文件夹中。将如下的应答组放入应答库中，即可通过 `.draw#...` 来调用牌堆。
```json
{
      "regex": "^\\.draw#",
      "anss":[
        {"order": "draw_deck", "msgread": true}
      ],
      "pri": 105
}
```
- 注意：并不完全兼容。需要对牌堆内部分特殊文本(如`{nick}`进行替换)

### 应答内容替换

一些特殊的字符串将会在输出前被替换成其他内容。部分如下：
- [name]
	- 替换为发信息者的群名片/昵称

### 开始使用：
- 请确保已完成所有配置
- ~~将Jiemeng的全部文件内容放到和go-cqhttp同一文件夹下，运行start.exe即可~~
- 分别启动 lagrange 与 桔梦。


---

## 已知问题

1. 各附加功能尚未完全支持
2. `aarch64` 环境下存在消息发送失败后程序崩溃退出的问题。未在 `x86_64` 环境下观察到。

