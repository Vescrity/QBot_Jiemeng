# QBot_Jiemeng
借助go-cqhttp，使用C++开发的QQ聊天机器人程序

- 本说明适用于v22.0
- 由于过于频繁的风控暂时放弃维护。~~相关版本尚未发布，请耐心等候。预计7月前会发布。~~
- 项目主要用于个人开发练习。
- 本说明用于介绍桔梦基本功能，组成，并帮助您快速上手。
- 具体的详细说明请参照进阶说明。

## 简介
桔梦(QBot_Jiemeng)是一个借助[go-cqhttp](https://github.com/Mrs4s/go-cqhttp)来达到QQ聊天功能的机器人程序。使用者可通过桔梦来实现检测关键词并做出特定回复的功能。词库可自定义。

## ~~开发者的一些废话~~
~~非计算机相关专业，纯业余开发，技术稀烂，代码可读性极差，总之图一乐~~

## 系统需求

Windows 64位 / Linux

## 构建

仅提供 Linux 编译方案：  
环境条件：确保安装boost库，以及安装了 g++。(测试了12/13版本)  
执行`./build.sh`可在`build`目录下构建基础的工作文件夹。

## 组成

桔梦的工作需要以下文件：
- 主体程序(默认名称Jiemeng)
	- 负责进行消息处理等任务
- `go-cqhttp` 及其相关文件
	- 用于接收、发送消息
- 启动器 `start` (可选)
	- 启动程序，若桔梦崩溃可提供自动重启功能
	- 当前稳定性相对较好，基本不需要
- symbol程序(搭配start)
	- 作为标记表明程序正常运行
- 配置文件 `Config.json`
- 词库文件
- 插件及其附属文件(非必须)
- 附加程序(非必需)


## 使用说明

### 使用前：

#### 下载

1. 下载新版桔梦release
2. 按release中说明下载并配置[go-cqhttp](https://github.com/Mrs4s/go-cqhttp)

#### 配置

以下适用于 Windows 配置参考。

1. 打开go-cq程序以生成.bat文件

2. 配置go-cqhttp相关配置[config.yml]
	- a.账号密码
	- b.监听端口与反向HTTP端口
		- 监听端口设置为任意空闲端口【参考：5702】
		- 反向HTTP POST：
			- url: `http://127.0.0.1:任意空闲端口（不可为监听端口）/`
			- 【参考：5703】
	- c.完成配置后请运行go-cqhttp.bat并按提示完成登陆

2. 配置Jiemeng相关配置[config.json]
	- (可参考预设文件)
	- 分为三个 json 对象：
  	- `Config`
  	- 
      | 字段             | 类型   | 说明                           | 默认值          |
      | ---------------- | ------ | ------------------------------ | --------------- |
      | Self_ID          | string | bot qq号                       | 0               |
      | REPORT_ID        | string | 上报人 qq号                    | 0               |
      | REPORT_GROUP     | string | 上报群 qq号                    | 0               |
      | INPORT           | int    | 反向HTTP中的端口号             | 必填            |
      | OUTPORT          | int    | 监听端口                       | 必填            |
      | sleep_time       | int    | 每次做出应答后的冷却时间(ms)   | 50              |
      | delay_time       | int    | 发送多条消息时的间隔时间(ms)   | 2000            |
      | Time_Check_Delay | int    | 定时消息时钟更新时间间隔(ms)   | 300000          |
      | MAX_TEXT_LENGTH  | int    | 文本形式发送的最大单条消息长度 | 2048            |
      | TITLE            | string | 程序标题                       | Jiemeng+Self_ID |
      | ANS_FILENAME     | string | 应答库文件名                   | Ans.json        |
      | SYMBOL_NAME      | string | symbol程序的文件名             | 必填            |

		- > 上报功能暂未支持

		- `List_Config`
		- 
      | 字段            | 类型       | 说明                                          | 默认值 |
      | --------------- | ---------- | --------------------------------------------- | ------ |
      | private_ifblack | bool       | 私聊名单是否作为黑名单                        | 必填   |
      | admin_list      | json_array | 管理员列表<br/>{"QQ号(string)":管理等级(int)} | 必填   |
      | private_list    | string[]   | 私聊名单                                      | 必填   |
      | black_list      | string[]   | 全局黑名单                                    | 必填   |
      | group_list      | string[]   | 群聊白名单                                    | 必填   |
      | broad_list      | string[]   | 私聊广播名单                                  | null   |
		- > 私聊广播功能未验证
		- `Custom_Config`
  		- 任意自定义量。

3. 配置start相关配置[start.cfg]
	- 第一行: Jiemeng主程序的文件名
	- 第二行: go-cqhttp的exe文件名
	- 第三行：是否显示来自go-cqhttp的信息(show/hide)
	- 第四行: symbol程序的文件名

#### 应答库文件


##### 结构

###### 应答组

应答库由若干个应答组组成。文件于v21.0版后开始采用json格式文件。

- 所有的应答组对象将作为一个对象数组保存在"Answers"字段中。

###### 应答组对象

| 字段  | 类型            | 描述                                                                                                                                                             |
| ----- | --------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| regex | string or array | 作为匹配规则。                                                                                                                                                   |
| anss  | array           | 结构复杂，参照进阶说明。                                                                                                                                         |
| level | int             | 生效等级                                                                                                                                                         |
| grps  | string[]        | 用于指定生效的群组范围。其中第一项为"1"指定为所有白名单群聊，"-1"指定为除后续指定群聊外的所有白名单群聊。否则指定为所列出的名单中所有白名单群聊                  |
| user  | string[]        | 用于指定生效的用户范围。其中第一项为"1"指定为所有有效用户，"-1"指定为除后续指定账号外的所有有效用户。"0"为不在私聊中生效。否则指定为所列出的名单中所有白名单群聊 |
| pri   | int             | 指定应答组的优先级                                                                                                                                               |

- regex 字段
	- regex 字段的内容在这里称为 正则元素 。
	- 一个合法的正则元素可以是一个正则表达式字符串，也可以是一个 正则元素 数组，也可以是一个包含 not 字段的对象。
	- 对于每个正则元素，都会对于消息产生 匹配 或 不匹配 的结果。
	- 一个正则元素数组的匹配结果是其内部的所有正则元素的 逻辑或 或者 逻辑且 的结果
	- regex 字段对应的 正则元素 若为数组，则该层逻辑为 逻辑或 。
	- 逻辑或 层内部的数组匹配逻辑为 逻辑且 。
	- 逻辑且 层内部的数组匹配逻辑为 逻辑或 。
	- not 字段对应的元素若为数组，其匹配逻辑为 逻辑或 。
	- not 对象的匹配结果为其内部元素结果取反。
	- regex 字段对应的 正则元素 不可为对象。
	- 匹配结果为 真 则执行此应答组输出，否则进行下一应答组匹配。

##### 逻辑

- 所有应答组按优先级由大至小排序。
- 收到消息后按顺序检查是否匹配应答组条件。
- 触发应答组后，将会对"anss"字段中的内容随机一条进行发送。


##### 文本内容常见替换
出于一些操作需求，一些特殊的字符串将会在输出前被替换成其他内容。部分如下：
- [poke]
	- 单独写成一条回复，表明发送一条戳一戳（头像双击）
- [name]
	- 替换为发信息者的群名片/昵称

### 开始使用：
- 请确保已完成所有配置
- 将Jiemeng的全部文件内容放到和go-cqhttp同一文件夹下，运行start.exe即可


---

## 已知问题

1. 各附加功能尚未完全支持
2. `aarch64` 环境下存在消息发送失败后程序崩溃退出的问题。未在 `x86_64` 环境下观察到。

