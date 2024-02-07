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

## 系统需求

Windows 64位 / Linux

## 构建

仅提供 Linux 编译方案：  
环境条件：确保安装boost库，curl 库，nlohmann json库，以及安装了 g++。(测试了12/13版本)   
例如 `Arch Linux` 下：
```
pacman -S boost nlohmann-json
# curl gcc 一般已预装，故不列出。
```
执行`./build.sh`可在`build`目录下构建基础的工作文件夹。

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

### 配置

【待补充】

1. 配置Jiemeng相关配置[config.json]
	- (可参考预设文件)
	- 分为三个 json 对象：
  	- `Config`
  	- 
      | 字段             | 类型   | 说明                                 | 默认值          |
      | ---------------- | ------ | ------------------------------------ | --------------- |
      | Self_ID          | string | bot qq号                             | 0               |
      | REPORT_ID        | string | 上报人 qq号                          | 0               |
      | REPORT_GROUP     | string | 上报群 qq号                          | 0               |
      | INPORT           | int    | ~~反向HTTP中的端口号~~ WebSocket端口 | 必填            |
      | OUTPORT          | int    | ~~监听端口~~无用，即将删除           | 必填            |
      | sleep_time       | int    | 每次做出应答后的冷却时间(ms)         | 50              |
      | delay_time       | int    | 发送多条消息时的间隔时间(ms)         | 2000            |
      | Time_Check_Delay | int    | 定时消息时钟更新时间间隔(ms)         | 300000          |
      | MAX_TEXT_LENGTH  | int    | 文本形式发送的最大单条消息长度       | 2048            |
      | TITLE            | string | 程序标题(Linux下暂时没有效果)        | Jiemeng+Self_ID |
      | SYMBOL_NAME      | string | ~~symbol程序的文件名~~ 无用，待移除  | 必填            |

		- > 上报功能暂未支持

		- `List_Config`
		- 
      | 字段            | 类型       | 说明                                          | 默认值                   |
      | --------------- | ---------- | --------------------------------------------- | ------------------------ |
      | private_ifblack | bool       | 私聊名单(private_list)是否作为黑名单          | 无默认，需保证类型是数组 |
      | admin_list      | json_array | 管理员列表<br/>{"QQ号(string)":管理等级(int)} | 无默认，需保证类型是数组 |
      | private_list    | string[]   | 私聊名单                                      | 无默认，需保证类型是数组 |
      | black_list      | string[]   | 全局黑名单                                    | 无默认，需保证类型是数组 |
      | group_list      | string[]   | 群聊白名单                                    | 无默认，需保证类型是数组 |
      | broad_list      | string[]   | 私聊广播名单                                  | null                     |
		- > 私聊广播功能未验证
		- `Custom_Config`
  		- 任意自定义量。
  		- 词库中所有形如 `{~...}` 的字符串会被替换为其对应的 `Custom_Config` 中设定的值。
  		- 例如你在 `Custom_Config` 中设定了 `"test": "1234"` ，那么你词库中所有的 `{~test}` 都会被替换为 `1234`




### 应答库文件


#### 应答组

应答库由若干个应答组组成。

- 所有的应答组对象将作为一个对象数组保存在"Answers"字段中。

#### 应答组对象

| 字段  | 类型            | 描述                                                                                                                                                             |
| ----- | --------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| regex | string or array | 匹配规则。                                                                                                                                                       |
| anss  | array           | 应答                                                                                                                                                             |
| level | int             | 生效等级，用于权限控制                                                                                                                                           |
| grps  | string[]        | 用于指定生效的群组范围。其中第一项为"1"指定为所有白名单群聊，"-1"指定为除后续指定群聊外的所有白名单群聊。否则指定为所列出的名单中所有白名单群聊                  |
| user  | string[]        | 用于指定生效的用户范围。其中第一项为"1"指定为所有有效用户，"-1"指定为除后续指定账号外的所有有效用户。"0"为不在私聊中生效。否则指定为所列出的名单中所有白名单群聊 |
| pri   | int             | 指定应答组的优先级                                                                                                                                               |

#### regex 字段
- regex 字段的内容在这里称为 `正则元素` 。
- 一个合法的正则元素可以是一个正则表达式字符串，也可以是一个 `正则元素` 数组，也可以是一个包含 `not` 字段的对象。
- 对于每个正则元素，都会对于消息产生 `匹配`(true) 或 `不匹配`(false) 的结果。
- 一个正则元素数组的匹配结果是其内部的所有正则元素的 逻辑或 或者 逻辑且 的结果
- 对于每个正则元素数组，我们定义一个概念叫做 `层逻辑`，上一行中提到的逻辑或/且即取决于当前层层逻辑的状态。
- `regex` 字段对应的 `正则元素` 若为数组，则该层的层逻辑为 `逻辑或`
- 逻辑或 层内部的数组匹配逻辑为 逻辑且 。
- 逻辑且 层内部的数组匹配逻辑为 逻辑或 。
- `not` 字段对应的元素若为数组，其匹配逻辑为 逻辑或 。
- `not` 对象的匹配结果为其内部运算结果取反。
- `regex` 字段对应的 正则元素 不可为对象(Object)，即只能为字符串或数组。
- 匹配结果为 真 则执行此应答组输出，否则进行下一应答组匹配。

从实例入手来看：

##### 实例 1

```
"regex": "早"
```

| 消息内容   | 匹配结果 |
| ---------- | -------- |
| 早         | true     |
| 早上好     | true     |
| 你走的真早 | true     |
| 晚上好     | false    |

##### 实例 2

```
"regex": ["早", {"not": "真早"}]
```

| 消息内容   | 匹配结果 | 说明                                             |
| ---------- | -------- | ------------------------------------------------ |
| 早         | true     | 逻辑或，两个子判断均为true（含有早，不含有真早） |
| 早上好     | true     | 两个子判断均为true                               |
| 你走的真早 | true     | true, false, 逻辑或结果为true                    |
| 晚上好     | true     | false, true, 逻辑或结果为true                    |

##### 实例 3

```
"regex": [["早", {"not": "真早"}]]
```

| 消息内容   | 匹配结果 | 说明                                           |
| ---------- | -------- | ---------------------------------------------- |
| 早         | true     | 由于位于最外层逻辑或的内层，为逻辑且，均为true |
| 早上好     | true     | 两个子判断均为true                             |
| 你走的真早 | false    | true, false, 逻辑且结果为 false                |
| 晚上好     | false    | false, true, 逻辑且结果为 false                |

##### 更多的合法 regex 字段

```
"regex": ["^生效群#","^\\.add_grp#"]
"regex": ["^\\.help$","^桔梦帮助$",["桔梦","使用说明"]]
"regex": [
  [
    "喜欢",["曲","歌","音乐"]
  ]
]
"regex": [
  [
    "吃.*(?:啥|什么)",{"not":"不"}
  ]
]
```

#### anss 字段

- `anss` 字段的内容为应答元素数组。
- 数组的每个元素可以是一个字符串表示最基本的消息输出，也可以是指令，或者是又一个应答元素数组
- 显然，又出现了与`regex`字段类似的嵌套。同样遵循同样的逻辑规则。
- 层逻辑为或的表示随机从这该层若干个应答元素中选取一个执行
- 为且的则表示执行该数组中的每一个元素。
- 与 `regex` 字段所不同的是，对于层逻辑为或的层，可能会有需求来指定每个元素的权重，以控制每个元素的随机概率
- 故需要提供一个 `weight` 参数。对于数组，则表示为一个对象，结构为`{"anss":[...],"weight":10}`
- 具体请见应答元素说明

#### 应答元素

应答元素用来描述这个应答操作。可分为两大类
- 文本输出
  - 直接使用字符串来表示该操作为执行文本输出。
  - 当需要提供 `weight` 参数时，表示为`{"text":"...","weight":10}`
- 指令操作
  - 结构为`{"order":"...."}` 其他参数与具体指令相关
  - 具体详见`指令`说明


#### 逻辑

- 所有应答组按优先级由大至小排序。
- 收到消息后按顺序检查是否匹配应答组条件。
- 触发应答组后，执行对应的应答元素操作
- 匹配到后，不继续进行后续匹配，除非执行的应答操作中执行了 ignore 指令

### Pre_Catch 动态优先应答组

考虑到可能需要的简单的上下文功能支持和特殊功能的实现引入此概念。

- 动态：这一部分的词库可随着使用过程的触发情况动态增添或删除
- 优先：这一部分总是优先于普通的应答组执行匹配。

#### 与普通应答组的字段区别

1. 不具有 `pri` 字段。优先顺序为添加到列表的先后顺序
2. 具有 `survive` 字段，表示一旦触发后是否将其从列表中移除
3. 具有 `type` 字段，定义其匹配范围，取代掉了 `grp` 和 `usr` 字段

#### 预制 Pre_Catch

在代码编译阶段便预定义了几个 Pre_Catch。其值分别为

| regex       | level | type  | survive | 功能                                                                    |
| ----------- | ----- | ----- | ------- | ----------------------------------------------------------------------- |
| "^/#"       | 100   | "all" | true    | 为Bot管理员提供一个通用的指令接口</br>使用`/#order#...`即可直接执行指令 |
| "^\\.info$" | 0     | "all" | true    | `.info` 显示bot信息                                                     |

#### 示例

可参见 sample 文件夹中的 `pre_catch_sample.json`

### 指令

#### reRecv

等效bot收到了指令参数中指定的内容

#### ignore

忽视本次匹已配成功的记录，即执行当前的应答的操作同时，继续向后匹配


### 文本内容常见替换
出于一些操作需求，一些特殊的字符串将会在输出前被替换成其他内容。部分如下：
- [poke]
	- 单独写成一条回复，表明发送一条戳一戳（头像双击）
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

