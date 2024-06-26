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
apt.globals.set("url", "http://10.140.32.106:10002");
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/文章

```text
与操作文章相关的API
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
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/文章/获取文章列表

```text
获取文章列表
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/articles/{page}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| page | 1 | 文章列表的分页页码 |

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
			"author": {
				"id": 1,
				"realname": "Bob Wood"
			},
			"category": {
				"id": 3,
				"name": "短篇小说",
				"slug": "short-story"
			},
			"create_time": "2023-01-18 23:53:41",
			"excerpt": "",
			"id": 100,
			"title": "山"
		},
		{
			"author": {
				"id": 2,
				"realname": "小华"
			},
			"category": {
				"id": 1,
				"name": "未分类",
				"slug": "uncategoried"
			},
			"create_time": "2022-12-30 07:20:41",
			"excerpt": "我的家乡是一座静谧而美丽的小镇，充满了勤劳和乐观的人们，他们热爱生活，创造着美好的未来。",
			"id": 97,
			"title": "我的家乡"
		},
		{
			"author": {
				"id": 5,
				"realname": "迪迪"
			},
			"category": {
				"id": 4,
				"name": "童话故事",
				"slug": "fairy-tale"
			},
			"create_time": "2022-12-28 06:00:02",
			"excerpt": "",
			"id": 96,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "莴苣姑娘"
		},
		{
			"author": {
				"id": 1,
				"realname": "Bob Wood"
			},
			"category": {
				"id": 4,
				"name": "童话故事",
				"slug": "fairy-tale"
			},
			"create_time": "2022-12-21 13:42:40",
			"excerpt": "",
			"id": 95,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "小红帽"
		},
		{
			"author": {
				"id": 2,
				"realname": "小华"
			},
			"category": {
				"id": 3,
				"name": "短篇小说",
				"slug": "short-story"
			},
			"create_time": "2022-12-07 22:49:16",
			"excerpt": "",
			"id": 93,
			"title": "这是一个超级长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长长的标题"
		},
		{
			"author": {
				"id": 2,
				"realname": "小华"
			},
			"category": {
				"id": 6,
				"name": "技术博文",
				"slug": "tech-blog"
			},
			"create_time": "2022-12-04 10:35:27",
			"excerpt": "",
			"id": 92,
			"tags": [
				{
					"id": 13,
					"name": "C++",
					"slug": "C++"
				}
			],
			"title": "C++智能指针"
		},
		{
			"author": {
				"id": 5,
				"realname": "迪迪"
			},
			"category": {
				"id": 4,
				"name": "童话故事",
				"slug": "fairy-tale"
			},
			"create_time": "2022-11-27 17:20:24",
			"excerpt": "",
			"id": 91,
			"title": "标题91"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"category": {
				"id": 1,
				"name": "未分类",
				"slug": "uncategoried"
			},
			"create_time": "2022-11-22 02:11:08",
			"excerpt": "",
			"id": 90,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "标题90"
		},
		{
			"author": {
				"id": 1,
				"realname": "Bob Wood"
			},
			"category": {
				"id": 2,
				"name": "诗歌",
				"slug": "poem"
			},
			"create_time": "2022-11-19 19:12:25",
			"excerpt": "",
			"id": 89,
			"tags": [
				{
					"id": 13,
					"name": "C++",
					"slug": "C++"
				}
			],
			"title": "月亮"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"category": {
				"id": 5,
				"name": "新闻",
				"slug": "news"
			},
			"create_time": "2022-11-18 19:03:29",
			"excerpt": "",
			"id": 88,
			"tags": [
				{
					"id": 4,
					"name": "Python",
					"slug": "python"
				},
				{
					"id": 13,
					"name": "C++",
					"slug": "C++"
				},
				{
					"id": 15,
					"name": "MySQL",
					"slug": "MySQL"
				}
			],
			"title": "标题88"
		}
	],
	"num_pages": 10,
	"status": 0
}
```

## /libre-blog/文章/获取某个分类下的文章列表

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
			"author": {
				"id": 1,
				"realname": "Bob Wood"
			},
			"create_time": "2022-11-19 19:12:25",
			"excerpt": "",
			"id": 89,
			"tags": [
				{
					"id": 13,
					"name": "C++",
					"slug": "C++"
				}
			],
			"title": "月亮"
		},
		{
			"author": {
				"id": 1,
				"realname": "Bob Wood"
			},
			"create_time": "2022-11-13 18:06:08",
			"excerpt": "",
			"id": 87,
			"title": "标题87"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"create_time": "2022-11-11 06:57:47",
			"excerpt": "",
			"id": 86,
			"title": "标题86"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"create_time": "2022-10-09 22:32:02",
			"excerpt": "",
			"id": 79,
			"title": "标题79"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"create_time": "2022-04-28 17:48:35",
			"excerpt": "",
			"id": 54,
			"title": "标题54"
		},
		{
			"author": {
				"id": 1,
				"realname": "Bob Wood"
			},
			"create_time": "2022-02-27 11:01:55",
			"excerpt": "",
			"id": 44,
			"title": "标题44"
		},
		{
			"author": {
				"id": 3,
				"realname": "小莫"
			},
			"create_time": "2022-01-20 22:16:55",
			"excerpt": "",
			"id": 36,
			"title": "标题36"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"create_time": "2021-12-31 09:49:51",
			"excerpt": "",
			"id": 34,
			"title": "标题34"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"create_time": "2021-10-27 17:25:33",
			"excerpt": "",
			"id": 23,
			"title": "标题23"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"create_time": "2021-09-14 12:30:43",
			"excerpt": "",
			"id": 16,
			"title": "标题16"
		}
	],
	"category": {
		"id": 2,
		"name": "诗歌",
		"slug": "poem"
	},
	"num_pages": 2,
	"status": 0
}
```

## /libre-blog/文章/获取某个用户下的文章列表

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/user/{id}/{page}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| id | 1 | 用户id |
| page | 1 | 分页页码 |

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
			"category": {
				"id": 3,
				"name": "短篇小说",
				"slug": "short-story"
			},
			"create_time": "2023-01-18 23:53:41",
			"excerpt": "",
			"id": 100,
			"title": "山"
		},
		{
			"category": {
				"id": 4,
				"name": "童话故事",
				"slug": "fairy-tale"
			},
			"create_time": "2022-12-21 13:42:40",
			"excerpt": "",
			"id": 95,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "小红帽"
		},
		{
			"category": {
				"id": 2,
				"name": "诗歌",
				"slug": "poem"
			},
			"create_time": "2022-11-19 19:12:25",
			"excerpt": "",
			"id": 89,
			"tags": [
				{
					"id": 13,
					"name": "C++",
					"slug": "C++"
				}
			],
			"title": "月亮"
		},
		{
			"category": {
				"id": 2,
				"name": "诗歌",
				"slug": "poem"
			},
			"create_time": "2022-11-13 18:06:08",
			"excerpt": "",
			"id": 87,
			"title": "标题87"
		},
		{
			"category": {
				"id": 1,
				"name": "未分类",
				"slug": "uncategoried"
			},
			"create_time": "2022-08-29 08:45:39",
			"excerpt": "",
			"id": 72,
			"title": "标题72"
		},
		{
			"category": {
				"id": 1,
				"name": "未分类",
				"slug": "uncategoried"
			},
			"create_time": "2022-07-21 16:24:42",
			"excerpt": "",
			"id": 67,
			"title": "标题67"
		},
		{
			"category": {
				"id": 5,
				"name": "新闻",
				"slug": "news"
			},
			"create_time": "2022-06-13 23:15:51",
			"excerpt": "",
			"id": 61,
			"title": "标题61"
		},
		{
			"category": {
				"id": 1,
				"name": "未分类",
				"slug": "uncategoried"
			},
			"create_time": "2022-04-26 08:53:16",
			"excerpt": "",
			"id": 53,
			"title": "标题53"
		},
		{
			"category": {
				"id": 5,
				"name": "新闻",
				"slug": "news"
			},
			"create_time": "2022-04-07 18:04:31",
			"excerpt": "",
			"id": 50,
			"title": "标题50"
		},
		{
			"category": {
				"id": 2,
				"name": "诗歌",
				"slug": "poem"
			},
			"create_time": "2022-02-27 11:01:55",
			"excerpt": "",
			"id": 44,
			"title": "标题44"
		}
	],
	"author": {
		"id": 1,
		"realname": "Bob Wood"
	},
	"num_pages": 2,
	"status": 0
}
```

## /libre-blog/文章/获取某个标签下的文章列表

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/tag/{slug}/{page}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| slug | green-brother | - |
| page | 1 | - |

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
			"author": {
				"id": 5,
				"realname": "迪迪"
			},
			"category": {
				"id": 4,
				"name": "童话故事",
				"slug": "fairy-tale"
			},
			"create_time": "2022-12-28 06:00:02",
			"excerpt": "",
			"id": 96,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "莴苣姑娘"
		},
		{
			"author": {
				"id": 1,
				"realname": "Bob Wood"
			},
			"category": {
				"id": 4,
				"name": "童话故事",
				"slug": "fairy-tale"
			},
			"create_time": "2022-12-21 13:42:40",
			"excerpt": "",
			"id": 95,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "小红帽"
		},
		{
			"author": {
				"id": 4,
				"realname": "Tom Smith"
			},
			"category": {
				"id": 1,
				"name": "未分类",
				"slug": "uncategoried"
			},
			"create_time": "2022-11-22 02:11:08",
			"excerpt": "",
			"id": 90,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "标题90"
		},
		{
			"author": {
				"id": 2,
				"realname": "小华"
			},
			"category": {
				"id": 4,
				"name": "童话故事",
				"slug": "fairy-tale"
			},
			"create_time": "2022-09-06 04:26:40",
			"excerpt": "",
			"id": 73,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "标题73"
		},
		{
			"author": {
				"id": 1,
				"realname": "Bob Wood"
			},
			"category": {
				"id": 4,
				"name": "童话故事",
				"slug": "fairy-tale"
			},
			"create_time": "2021-10-03 02:16:32",
			"excerpt": "",
			"id": 18,
			"tags": [
				{
					"id": 10,
					"name": "格林童话",
					"slug": "green-brother"
				}
			],
			"title": "标题18"
		}
	],
	"num_pages": 1,
	"status": 0,
	"tag": {
		"id": 10,
		"name": "格林童话",
		"slug": "green-brother"
	}
}
```

## /libre-blog/文章/后台管理获取文章列表

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/admin/articles/{page}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| page | 1 | 文章列表的分页页码 |

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/文章/获取文章

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

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| id | 95 | 文章id |

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
		"author": {
			"id": 1,
			"realname": "Bob Wood"
		},
		"category": {
			"id": 4,
			"name": "童话故事",
			"slug": "fairy-tale"
		},
		"content": "",
		"create_time": "2022-12-21 13:42:40",
		"excerpt": "",
		"id": 95,
		"tags": [
			{
				"id": 10,
				"name": "格林童话",
				"slug": "green-brother"
			}
		],
		"title": "小红帽"
	},
	"status": 0
}
```

## /libre-blog/文章/添加文章

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/article/add

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "title": "一个小人物",
    "category": 25,
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/文章/更新文章

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

## /libre-blog/文章/删除文章

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

#### 请求Body参数

```javascript
{
	"ids": [
		8, 9, 10, 11
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

## /libre-blog/分类

```text
与操作分类相关的API
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
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/分类/获取分类列表

```text
获取全部分类信息，预期返回一个包含多个分类信息的数组。
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/categories

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
			"id": 2,
			"name": "诗歌",
			"slug": "poetry"
		},
		{
			"id": 3,
			"name": "小说",
			"slug": "novel"
		},
		{
			"id": 4,
			"name": "散文",
			"slug": "prose"
		},
		{
			"id": 5,
			"name": "随笔",
			"slug": "essay"
		},
		{
			"id": 6,
			"name": "技术文章",
			"slug": "tech-art"
		}
	],
	"status": 0
}
```

## /libre-blog/分类/获取分类

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/category/{id}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| id | 3 | 分类的id |

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
	"category": {
		"id": 3,
		"name": "小说",
		"slug": "novel"
	},
	"status": 0
}
```

## /libre-blog/分类/添加分类

```text
添加分类。
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/category/add

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "name": "散文",
    "slug": "prose"
}
```

| 参数名 | 示例值 | 参数类型 | 是否必填 | 参数描述 |
| --- | --- | ---- | ---- | ---- |
| name | 散文 | String | 是 | 分类的名称 |
| slug | prose | String | 是 | 分类的缩略名 |

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/分类/更新分类信息

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/category/update

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "id": 2,
    "name": "诗歌",
    "slug": "poem"
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/分类/删除分类

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/category/delete

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "ids": [
        1
    ]
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/标签

```text
与操作标签相关的API
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
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/标签/获取标签列表

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/tags/{page}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
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
	"num_pages": 2,
	"status": 0,
	"tags": [
		{
			"id": 1,
			"name": "新闻",
			"num_articles": 0,
			"slug": "news"
		},
		{
			"id": 2,
			"name": "格林童话",
			"num_articles": 0,
			"slug": "green-brother"
		},
		{
			"id": 3,
			"name": "C++",
			"num_articles": 1,
			"slug": "cpp"
		},
		{
			"id": 4,
			"name": "Python",
			"num_articles": 0,
			"slug": "python"
		},
		{
			"id": 5,
			"name": "后端开发",
			"num_articles": 0,
			"slug": "backend-dev"
		},
		{
			"id": 6,
			"name": "前端开发",
			"num_articles": 0,
			"slug": "frontend-dev"
		},
		{
			"id": 7,
			"name": "Rust",
			"num_articles": 0,
			"slug": "rust"
		},
		{
			"id": 8,
			"name": "短篇小说",
			"num_articles": 1,
			"slug": "short-story"
		},
		{
			"id": 9,
			"name": "科幻小说",
			"num_articles": 0,
			"slug": "sci-fi"
		},
		{
			"id": 10,
			"name": "刘慈欣",
			"num_articles": 0,
			"slug": "liu-ci-xin"
		}
	]
}
```

## /libre-blog/标签/获取标签信息

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/tag/{id}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| id | 2 | 标签的id |

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
	"tag": {
		"id": 2,
		"name": "格林童话",
		"slug": "green-brother"
	}
}
```

## /libre-blog/标签/添加标签

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/tag/add

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "name": "悬疑小说",
    "slug": "suspense-novel"
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/标签/更新标签信息

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/tag/update

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "id": 6,
    "name": "数据库",
    "slug": "database" 
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/标签/删除标签

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/tag/delete

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "ids": [
        
    ]
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/用户

```text
与操作用户相关的API
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
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/用户/用户登录

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
    "username": "bobwood",
    "password": "bw1234"
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

## /libre-blog/用户/用户注册

```text
用户注册。请求json中需要包含username，email和passowrd字段。
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/user/register

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

## /libre-blog/用户/个人中心

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
		"create_time": "2024-05-09 08:01:11",
		"email": "bobwood@ouc.edu.cn",
		"id": 1,
		"realname": "Bob Wood",
		"role": "administrator",
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

## /libre-blog/用户/个人中心更新用户常规信息

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

## /libre-blog/用户/个人中心更新用户密码

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

#### 请求Body参数

```javascript
{
	"password": "bw1234"
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

## /libre-blog/用户/获取用户列表

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/users/{page}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
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
	"num_pages": 1,
	"status": 0,
	"users": [
		{
			"create_time": "2024-05-09 08:01:11",
			"email": "bobwood@ouc.edu.cn",
			"id": 1,
			"realname": "Bob Wood",
			"role": "administrator",
			"username": "bobwood"
		},
		{
			"create_time": "2024-05-09 08:37:47",
			"email": "mike@gmail.com",
			"id": 2,
			"realname": "Mike Black",
			"role": "contributor",
			"username": "mike"
		},
		{
			"create_time": "2024-05-09 08:45:38",
			"email": "xiaoshuai@abc.org",
			"id": 3,
			"realname": "小帅",
			"role": "subscriber",
			"username": "xiaoshuai"
		},
		{
			"create_time": "2024-05-09 08:46:22",
			"email": "xiaomei@xxoo.hub",
			"id": 4,
			"realname": "小美",
			"role": "editor",
			"username": "xiaomei"
		},
		{
			"create_time": "2024-05-09 09:00:11",
			"email": "xiaohua@163.com",
			"id": 5,
			"realname": "xiaohua",
			"role": "subscriber",
			"username": "xiaohua"
		}
	]
}
```

## /libre-blog/用户/后台获取用户信息

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/user/{id}

#### 请求方式

> GET

#### Content-Type

> none

#### 路径变量

| 参数名 | 示例值 | 参数描述 |
| --- | --- | ---- |
| id | 2 | 用户id |

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
		"email": "mike@gmail.com",
		"id": 2,
		"realname": "Mike Black",
		"role": {
			"id": 3,
			"name": "contributor"
		},
		"username": "mike"
	}
}
```

## /libre-blog/用户/删除用户

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/user/delete

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "ids": [
        11, 13
    ]
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/用户/后台添加用户

```text
暂无描述
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
    "username": "jack",
    "password": "jack1234",
    "email": "jack@gmail.com"
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/用户/后台更新用户

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/user/update

#### 请求方式

> POST

#### Content-Type

> json

#### 请求Body参数

```javascript
{
    "id": 13,
    "username": "jack",
    "password": "",
    "email": "jack@gmail.com",
    "role": 4
}
```

#### 预执行脚本

```javascript
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/用户/获取当前用户的角色信息

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/user/role

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
	"role": {
		"roleId": 1,
		"roleName": "administrator"
	},
	"status": 0
}
```

## /libre-blog/角色

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
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/角色/获取角色列表

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/roles

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
	"roles": [
		{
			"id": 1,
			"name": "administrator"
		},
		{
			"id": 2,
			"name": "editor"
		},
		{
			"id": 3,
			"name": "contributor"
		},
		{
			"id": 4,
			"name": "subscriber"
		}
	],
	"status": 0
}
```

## /libre-blog/菜单

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
暂无预执行脚本
```

#### 后执行脚本

```javascript
暂无后执行脚本
```

## /libre-blog/菜单/获取后台菜单

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/admin/menu

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
	"menu": [
		{
			"children": [
				{
					"key": "userCenter",
					"label": "个人中心"
				}
			],
			"key": "console",
			"label": "控制台"
		},
		{
			"children": [
				{
					"key": "article",
					"label": "文章"
				},
				{
					"key": "user",
					"label": "用户"
				},
				{
					"key": "category",
					"label": "分类"
				},
				{
					"key": "tag",
					"label": "标签"
				}
			],
			"key": "manage",
			"label": "管理"
		},
		{
			"key": "index",
			"label": "首页"
		},
		{
			"key": "logout",
			"label": "登出"
		}
	],
	"status": 0
}
```

## /libre-blog/菜单/获取前台菜单

```text
暂无描述
```

#### 接口状态

> 开发中

#### 接口URL

> {{url}}/blog/menu

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
	"menu": [
		{
			"key": "index",
			"label": "首页"
		},
		{
			"key": "categories",
			"label": "分类"
		}
	],
	"status": 0
}
```
