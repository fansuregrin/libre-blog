## /libre-blog

```text
暂无描述
```

#### 公共Header参数

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| 暂无参数 |

#### 公共Query参数

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| 暂无参数 |

#### 公共Body参数

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| 暂无参数 |

#### 预执行脚本

```javascript
apt.environment.set("url", "http://10.140.32.106:10002");
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

> {{url}}/blog/page/{page}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| page | 1 | 文章列表分页页码，数字类型 |

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

> {{url}}/blog/article/{id}

#### 请求方式

> GET

#### Content-Type

> none

#### 请求Header参数

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NTIwNDEsImlhdCI6MTcxNDY0ODQ0MSwiaXNzIjoiZHJvZ29uIiwidWlkIjozfQ.8986kvIariMlOifEwMQqUd0rNGls3XJr6aZ0Xbg4dfw17UJawAYBh0nsVbH97Wp0tpF3e3ciCzGzXO0mfy5w2w | String | 是 | 用户token |

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| id | 84 | 文章id |

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

> {{url}}/blog/article/update

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Header参数

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NTIwNDEsImlhdCI6MTcxNDY0ODQ0MSwiaXNzIjoiZHJvZ29uIiwidWlkIjozfQ.8986kvIariMlOifEwMQqUd0rNGls3XJr6aZ0Xbg4dfw17UJawAYBh0nsVbH97Wp0tpF3e3ciCzGzXO0mfy5w2w | String | 否 | 用户的token |

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

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| id | 84 | Integer | 是 | 文章的id |
| title | 标题84 | String | 是 | 文章的标题 |
| author | 3 | Integer | 是 | 文章的作者id |
| category | 5 | Integer | 是 | 文章的分类id |
| excerpt | - | String | 是 | 文章的摘要 |
| content | 我是内容84 | String | 是 | 文章内容 |
| tags | - | Array | 否 | 文章的标签 |

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

> {{url}}/blog/article/delete

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Header参数

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ0NzkyODQsImlhdCI6MTcxNDQ3NTY4NCwiaXNzIjoiZHJvZ29uIiwidWlkIjozfQ.JLBCPy58o1W0DEd4wFrLDbU2AylA5srB1hwdY7CQw_MNHHSbntUG4ATBm1-wwE5tEWU0TMZFZe1-cLK_bRtdZg | String | 是 | 用户token |

#### 请求Body参数

```javascript
{
	"ids": [
		1,
		2
	]
}
```

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| ids | 1 | Array | 是 | 需要删除的文章id |

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

> {{url}}/blog/category

#### 请求方式

> GET

#### Content-Type

> none

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

> {{url}}/blog/category/{slug}/{page}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| slug | poem | 分类的缩略名 |
| page | 1 | 分页的页码 |

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

> {{url}}/user/center

#### 请求方式

> GET

#### Content-Type

> none

#### 请求Header参数

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NjA5ODQsImlhdCI6MTcxNDY1NzM4NCwiaXNzIjoiZHJvZ29uIiwidWlkIjoxfQ.hEWGno-4R_ISpMMb9Y_wkKZaDIuJUGFdPEpSInfZYf-jjOm5YTxOvdB7RlGbCrKsMMWydzPP76kwnSDzR1hlYA | String | 是 | 客户端保存的用户token，由服务端签发 |

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

| 参数名 | 示例值 | 参数类型 | 参数描述 |
| --- | --- | ---- | ---- |
| status | 0 | Integer | 状态码 |
| user | - | Object | 用户信息 |
| user.create_time | 2024-04-25 04:02:59 | String | 用户的创建时间 |
| user.email | bobwood@ouc.edu.cn | String | 用户的电子邮箱 |
| user.id | 1 | Integer | 用户的id |
| user.realname | Bob Wood | String | 用户的真实名称 |
| user.username | bobwood | String | 用户登录时使用的用户名 |

## /libre-blog/用户登录

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/login

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "username": "rbwhite",
    "password": "rbw1234"
}
```

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| username | bobwood | String | 是 | 用户名 |
| password | bw123456 | String | 是 | 用户密码 |

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

| 参数名 | 示例值 | 参数类型 | 参数描述 |
| --- | --- | ---- | ---- |
| status | 0 | Integer | 状态码 |
| token | eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ2NjA5ODQsImlhdCI6MTcxNDY1NzM4NCwiaXNzIjoiZHJvZ29uIiwidWlkIjoxfQ.hEWGno-4R_ISpMMb9Y_wkKZaDIuJUGFdPEpSInfZYf-jjOm5YTxOvdB7RlGbCrKsMMWydzPP76kwnSDzR1hlYA | String | 用户token |

## /libre-blog/添加用户

```text
添加新用户。请求json中需要包含username，email和passowrd字段。
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/user/add

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "username": "rbwhite",
    "email": "rbw@outlook.com",
    "password": "rbw1234",
    "realname": "R. B. Withe"
}
```

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| username | rbwhite | String | 是 | 用户名 |
| email | rbw@outlook.com | String | 是 | 用户的电子邮箱 |
| password | rbw12345 | String | 是 | 用户的密码 |
| realname | R. B. Withe | String | 是 | 用户的真实名称 |

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
	"message": "注册成功",
	"status": 0
}
```

| 参数名 | 示例值 | 参数类型 | 参数描述 |
| --- | --- | ---- | ---- |
| message | 注册成功 | String | 成功消息 |
| status | 0 | Integer | 状态码 |

## /libre-blog/更新用户常规信息

```text
更新用户的常规信息。常规信息是指用户的用户名、真实名称、邮箱等，但是不包括密码。更新密码使用单独的api。
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/user/update

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Header参数

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ3MTE1NTYsImlhdCI6MTcxNDcwNzk1NiwiaXNzIjoiZHJvZ29uIiwidWlkIjoxMX0.vsLtgFJdpqTIvZPxB4WrfZ3Ov8G9HToiMUBDPtT3JZZsdkpprnmL7DB2wlAEcKzBN_cBZy67vcjSXa3BFUtVMA | String | 是 | 用户token |

#### 请求Body参数

```javascript
{
    "username": "rbwhite",
    "email": "rbw@outlook.com",
    "realname": "R. B. White"
}
```

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| username | rbwhite | String | 是 | 用户名 |
| email | rbw@outlook.com | String | 是 | 用户的电子邮箱 |
| realname | R. B. White | String | 是 | 用户的真实名称 |

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

## /libre-blog/更新用户密码

```text
更新用户密码。
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/user/update/password

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Header参数

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| Authorization | Bearer eyJhbGciOiJFUzI1NksiLCJ0eXAiOiJKV1QifQ.eyJleHAiOjE3MTQ3MjE0NTQsImlhdCI6MTcxNDcxNzg1NCwiaXNzIjoiZHJvZ29uIiwidWlkIjoxMX0.71rK8nKW0IySH9eNsVALy2s8VpsWcD6mWaPmWF9Tgly2E3X7pb95Bz7DXWBKPO1AZiilpaQfT4bjnzPPIiN_LA | String | 是 | - |

#### 请求Body参数

```javascript
{
	"password": "rbw123456"
}
```

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| password | 123456 | String | 否 | 密码 |

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
