## 全局公共参数
#### 全局Header参数
参数名 | 示例值 | 参数描述
--- | --- | ---
暂无参数
#### 全局Query参数
参数名 | 示例值 | 参数描述
--- | --- | ---
暂无参数
#### 全局Body参数
参数名 | 示例值 | 参数描述
--- | --- | ---
暂无参数
#### 全局认证方式
```text
noauth
```
#### 全局预执行脚本
```javascript
暂无预执行脚本
```
#### 全局后执行脚本
```javascript
暂无后执行脚本
```
## /libre-blog
```text
暂无描述
```
#### Header参数
参数名 | 示例值 | 参数描述
--- | --- | ---
暂无参数
#### Query参数
参数名 | 示例值 | 参数描述
--- | --- | ---
暂无参数
#### Body参数
参数名 | 示例值 | 参数描述
--- | --- | ---
暂无参数
#### 认证方式
```text
bearer
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
## /libre-blog/获取文章列表
```text
获取文章列表
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/blog/page/{page}

#### 请求方式
> GET

#### Content-Type
> none

#### 路径变量
参数名 | 示例值 | 参数描述
--- | --- | ---
page | 1 | 文章列表分页页码，数字类型
#### 认证方式
```text
noauth
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
#### 成功响应示例
```javascript
{
	"articles": [
		{
			"author": 1,
			"author_name": "Bob Wood",
			"category": 3,
			"category_name": "短篇小说",
			"create_time": "2023-01-18",
			"excerpt": "",
			"id": 100,
			"title": "山"
		},
		{
			"author": 2,
			"author_name": "小华",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-12-30",
			"excerpt": "我的家乡是一座静谧而美丽的小镇，充满了勤劳和乐观的人们，他们热爱生活，创造着美好的未来。",
			"id": 97,
			"title": "我的家乡"
		},
		{
			"author": 5,
			"author_name": "迪迪",
			"category": 4,
			"category_name": "童话故事",
			"create_time": "2022-12-28",
			"excerpt": "",
			"id": 96,
			"title": "标题96"
		},
		{
			"author": 1,
			"author_name": "Bob Wood",
			"category": 4,
			"category_name": "童话故事",
			"create_time": "2022-12-21",
			"excerpt": "",
			"id": 95,
			"title": "小红帽"
		},
		{
			"author": 3,
			"author_name": "小莫",
			"category": 4,
			"category_name": "童话故事",
			"create_time": "2022-12-14",
			"excerpt": "",
			"id": 94,
			"title": "标题94"
		},
		{
			"author": 2,
			"author_name": "小华",
			"category": 3,
			"category_name": "短篇小说",
			"create_time": "2022-12-07",
			"excerpt": "",
			"id": 93,
			"title": "标题93"
		},
		{
			"author": 2,
			"author_name": "小华",
			"category": 1,
			"category_name": "未分类",
			"create_time": "2022-12-04",
			"excerpt": "",
			"id": 92,
			"title": "C++智能指针"
		},
		{
			"author": 5,
			"author_name": "迪迪",
			"category": 4,
			"category_name": "童话故事",
			"create_time": "2022-11-27",
			"excerpt": "",
			"id": 91,
			"title": "标题91"
		},
		{
			"author": 4,
			"author_name": "Tom Smith",
			"category": 1,
			"category_name": "未分类",
			"create_time": "2022-11-22",
			"excerpt": "",
			"id": 90,
			"title": "标题90"
		},
		{
			"author": 1,
			"author_name": "Bob Wood",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-11-19",
			"excerpt": "",
			"id": 89,
			"tags": [
				"C++"
			],
			"title": "月亮"
		}
	],
	"num_pages": 10,
	"status": 0
}
```
## /libre-blog/获取文章
```text
获取单篇文章的信息
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/blog/article/{id}

#### 请求方式
> GET

#### Content-Type
> none

#### 请求Header参数
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NTIwNDEsImlhdCI6MTcxNDY0ODQ0MSwiaXNzIjoiZHJvZ29uIiwidWlkIjozfQ.8986kvIariMlOifEwMQqUd0rNGls3XJr6aZ0Xbg4dfw17UJawAYBh0nsVbH97Wp0tpF3e3ciCzGzXO0mfy5w2w | String | 是 | 用户token
#### 路径变量
参数名 | 示例值 | 参数描述
--- | --- | ---
id | 84 | 文章id
#### 认证方式
```text
bearer
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
#### 成功响应示例
```javascript
{
	"article": {
		"author": 3,
		"author_name": "小莫",
		"category": 5,
		"category_name": "新闻",
		"content": "我是内容84",
		"create_time": "2022-11-03",
		"editable": false,
		"excerpt": "",
		"id": 84,
		"title": "标题84"
	},
	"status": 0
}
```
## /libre-blog/更新文章
```text
更新文章信息，如标题、分类、标签、内容、摘要等。
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/blog/article/update

#### 请求方式
> POST

#### Content-Type
> json

#### 请求Header参数
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NTIwNDEsImlhdCI6MTcxNDY0ODQ0MSwiaXNzIjoiZHJvZ29uIiwidWlkIjozfQ.8986kvIariMlOifEwMQqUd0rNGls3XJr6aZ0Xbg4dfw17UJawAYBh0nsVbH97Wp0tpF3e3ciCzGzXO0mfy5w2w | String | 否 | 用户的token
#### 请求Body参数
```javascript
{
	"id": 84,
	"title": "标题84",
	"author": 3,
	"category": 5,
	"excerpt": "",
	"content": "我是内容84",
	"tags": [
		"C++", "Web Development"
	]
}
```
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
id | 84 | Integer | 是 | 文章的id
title | 标题84 | String | 是 | 文章的标题
author | 3 | Integer | 是 | 文章的作者id
category | 5 | Integer | 是 | 文章的分类id
excerpt | - | String | 是 | 文章的摘要
content | 我是内容84 | String | 是 | 文章内容
tags | - | Array | 否 | 文章的标签
#### 认证方式
```text
bearer
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
#### 成功响应示例
```javascript
{
	"status": 0
}
```
## /libre-blog/删除文章
```text
删除指定id的文章，可以指定多个id
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/blog/article/delete

#### 请求方式
> POST

#### Content-Type
> json

#### 请求Header参数
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ0NzkyODQsImlhdCI6MTcxNDQ3NTY4NCwiaXNzIjoiZHJvZ29uIiwidWlkIjozfQ.JLBCPy58o1W0DEd4wFrLDbU2AylA5srB1hwdY7CQw_MNHHSbntUG4ATBm1-wwE5tEWU0TMZFZe1-cLK_bRtdZg | String | 是 | 用户token
#### 请求Body参数
```javascript
{
	"ids": [
		1,
		2
	]
}
```
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
ids | 1 | Array | 是 | 需要删除的文章id
#### 认证方式
```text
bearer
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
#### 成功响应示例
```javascript
{
	"status": 0
}
```
## /libre-blog/获取分类信息
```text
获取全部分类信息，预期返回一个包含多个分类信息的数组。
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/blog/category

#### 请求方式
> GET

#### Content-Type
> none

#### 认证方式
```text
noauth
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
#### 成功响应示例
```javascript
{
	"categories": [
		{
			"id": 1,
			"name": "未分类",
			"slug": "uncategoried"
		},
		{
			"id": 2,
			"name": "诗歌",
			"slug": "poem"
		},
		{
			"id": 3,
			"name": "短篇小说",
			"slug": "short-story"
		},
		{
			"id": 4,
			"name": "童话故事",
			"slug": "fairy-tale"
		},
		{
			"id": 5,
			"name": "新闻",
			"slug": "news"
		},
		{
			"id": 6,
			"name": "技术博文",
			"slug": "tech-blog"
		}
	],
	"status": 0
}
```
## /libre-blog/获取某个分类下的文章列表
```text
获取指定分类下的指定页的文章列表。
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/blog/category/{slug}/{page}

#### 请求方式
> GET

#### Content-Type
> none

#### 路径变量
参数名 | 示例值 | 参数描述
--- | --- | ---
slug | poem | 分类的缩略名
page | 1 | 分页的页码
#### 认证方式
```text
noauth
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
#### 成功响应示例
```javascript
{
	"articles": [
		{
			"author": 2,
			"author_name": "小华",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-12-30",
			"excerpt": "我的家乡是一座静谧而美丽的小镇，充满了勤劳和乐观的人们，他们热爱生活，创造着美好的未来。",
			"id": 97,
			"title": "我的家乡"
		},
		{
			"author": 1,
			"author_name": "Bob Wood",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-11-19",
			"excerpt": "",
			"id": 89,
			"tags": [
				"C++"
			],
			"title": "月亮"
		},
		{
			"author": 1,
			"author_name": "Bob Wood",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-11-13",
			"excerpt": "",
			"id": 87,
			"title": "标题87"
		},
		{
			"author": 4,
			"author_name": "Tom Smith",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-11-11",
			"excerpt": "",
			"id": 86,
			"title": "标题86"
		},
		{
			"author": 4,
			"author_name": "Tom Smith",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-10-09",
			"excerpt": "",
			"id": 79,
			"title": "标题79"
		},
		{
			"author": 4,
			"author_name": "Tom Smith",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-04-28",
			"excerpt": "",
			"id": 54,
			"title": "标题54"
		},
		{
			"author": 1,
			"author_name": "Bob Wood",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-02-27",
			"excerpt": "",
			"id": 44,
			"title": "标题44"
		},
		{
			"author": 3,
			"author_name": "小莫",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2022-01-20",
			"excerpt": "",
			"id": 36,
			"title": "标题36"
		},
		{
			"author": 4,
			"author_name": "Tom Smith",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2021-12-31",
			"excerpt": "",
			"id": 34,
			"title": "标题34"
		},
		{
			"author": 4,
			"author_name": "Tom Smith",
			"category": 2,
			"category_name": "诗歌",
			"create_time": "2021-10-27",
			"excerpt": "",
			"id": 23,
			"title": "标题23"
		}
	],
	"name": "诗歌",
	"num_pages": 2,
	"status": 0
}
```
## /libre-blog/获取用户信息
```text
暂无描述
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/user/center

#### 请求方式
> GET

#### Content-Type
> none

#### 请求Header参数
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NjA5ODQsImlhdCI6MTcxNDY1NzM4NCwiaXNzIjoiZHJvZ29uIiwidWlkIjoxfQ.hEWGno-4R_ISpMMb9Y_wkKZaDIuJUGFdPEpSInfZYf-jjOm5YTxOvdB7RlGbCrKsMMWydzPP76kwnSDzR1hlYA | String | 是 | 客户端保存的用户token，由服务端签发
#### 认证方式
```text
bearer
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
#### 成功响应示例
```javascript
{
	"status": 0,
	"user": {
		"create_time": "2024-04-25 04:02:59",
		"email": "bobwood@ouc.edu.cn",
		"id": 1,
		"realname": "Bob Wood",
		"username": "bobwood"
	}
}
```
参数名 | 示例值 | 参数类型 | 参数描述
--- | --- | --- | ---
status | 0 | Integer | 状态码
user | - | Object | 用户信息
user.create_time | 2024-04-25 04:02:59 | String | 用户的创建时间
user.email | bobwood@ouc.edu.cn | String | 用户的电子邮箱
user.id | 1 | Integer | 用户的id
user.realname | Bob Wood | String | 用户的真实名称
user.username | bobwood | String | 用户登录时使用的用户名
## /libre-blog/用户登录
```text
暂无描述
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/login

#### 请求方式
> POST

#### Content-Type
> json

#### 请求Body参数
```javascript
{
    "username": "bobwood",
    "password": "bw123456"
}
```
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
username | bobwood | String | 是 | 用户名
password | bw123456 | String | 是 | 用户密码
#### 认证方式
```text
noauth
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
#### 成功响应示例
```javascript
{
	"status": 0,
	"token": "eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NjA5ODQsImlhdCI6MTcxNDY1NzM4NCwiaXNzIjoiZHJvZ29uIiwidWlkIjoxfQ.hEWGno-4R_ISpMMb9Y_wkKZaDIuJUGFdPEpSInfZYf-jjOm5YTxOvdB7RlGbCrKsMMWydzPP76kwnSDzR1hlYA"
}
```
参数名 | 示例值 | 参数类型 | 参数描述
--- | --- | --- | ---
status | 0 | Integer | 状态码
token | eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NjA5ODQsImlhdCI6MTcxNDY1NzM4NCwiaXNzIjoiZHJvZ29uIiwidWlkIjoxfQ.hEWGno-4R_ISpMMb9Y_wkKZaDIuJUGFdPEpSInfZYf-jjOm5YTxOvdB7RlGbCrKsMMWydzPP76kwnSDzR1hlYA | String | 用户token
## /libre-blog/添加用户
```text
暂无描述
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/user/add

#### 请求方式
> POST

#### Content-Type
> json

#### 请求Body参数
```javascript
{
    "username": "R.B. White",
    "email": "rbw@outlook.com",
    "password": "rbw12345",
}
```
#### 认证方式
```text
noauth
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
## /libre-blog/更新用户常规信息
```text
暂无描述
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/user/update

#### 请求方式
> POST

#### Content-Type
> json

#### 请求Header参数
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQzODI5MDcsImlhdCI6MTcxNDM3OTMwNywiaXNzIjoiZHJvZ29uIiwidWlkIjozfQ.p9cnIXyJYRVcxqUXlLYm1tPa92ZvcHY1RVSv6uqRCCidikpPpK27I7Uj4qimjxw4IAF3pcyHsms1uZpEUJZ0dQ | String | 是 | -
#### 请求Body参数
```javascript
{
    "username": "xiaohua",
    "email": "xiaohua@bnu.edu.cn"
}
```
#### 认证方式
```text
bearer
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```
## /libre-blog/更新用户密码
```text
暂无描述
```
#### 接口状态
> 开发中

#### 接口URL
> http://10.140.32.106:10002/user/update/password

#### 请求方式
> POST

#### Content-Type
> json

#### 请求Header参数
参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述
--- | --- | --- | --- | ---
Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQzODU2MjksImlhdCI6MTcxNDM4MjAyOSwiaXNzIjoiZHJvZ29uIiwidWlkIjozfQ.3xMAmixLueWQmUyvW0QYQ0b_hZ7IpiqotoEBXexLkb0yMhonCR8IiiLvgOyFaImJoCH1GgNN5-h7S5Wr6i67PA | String | 是 | -
#### 请求Body参数
```javascript
{
    "password": "moy"
}
```
#### 认证方式
```text
bearer
```
#### 预执行脚本
```javascript
暂无预执行脚本
```
#### 后执行脚本
```javascript
暂无后执行脚本
```