# 🚀 Boost_Searcher

> **CppReference 搜索引擎**

## 🌟 版本更新
**V1.0.1**: 不再使用 Jsoncpp 作为序列化方案，改为使用 Protobuf。

**注意添加库路径**
```bash
export LD_LIBRARY_PATH=/usr/local/protobuf/lib:$LD_LIBRARY_PATH
```

---

## 📌 项目背景
🔍 站内搜索，数据更垂直，数据量更小，提升检索效率。

---

## 🛠 技术栈 & 项目环境
- **语言**：C/C++（C++11）.
- **库**：STL、Boost、Protobuf、cppjieba.
- **环境**：Ubuntu 22.04、g++/Makefile、VSCode.

---

## ⚙️ 项目原理

### 🌍 服务端
1. 下载 CppReference 对应的 HTML 文件。
2. 解析 HTML，清理无用数据，去除标签。
3. 建立 **正向索引** 和 **倒排索引**，加速网页信息查找。
4. 通过索引检索相关 HTML，组合内容。
5. 返回拼接后的搜索结果页面。

### 👤 客户端
用户发送 HTTP **GET** 请求，上传搜索关键字。

---

## 🔧 功能模块

### 📜 数据筛选（`parse.cc`）
📂 **使用 Boost 文件系统库** (`boost::filesystem`)
- **文件查找**：遍历 HTML 文件，存入 `file_list`。
- **解析内容**：提取 `title`、`content`、`url`，存入 `results`。
- **数据拼接**：以 `\3` 作为分隔符拼接不同文件。

### 📖 索引构建（`index.hpp`）
🛠 **核心组件：索引管理器（单例模式）**
- **数据结构**：
  - `DocInfo`：包含 `title`、`content`、`url`、`doc_id`
  - `InvertedElem`：包含 `doc_id`、`word`、`weight`
- **正向索引**：`vector` 结构，按 `id` 存储 `DocInfo`
- **倒排索引**：`unordered_map`，key 为 `string`，value 为 `vector<InvertedElem>`

### 🌐 服务器构建（`http_server.cc`）
🔗 **使用 `httplib` 处理 HTTP 请求**
- **处理请求**：解析查询参数，调用 `Searcher` 进行搜索。
- **返回 JSON**：使用 `Protobuf` 组装搜索结果。

---

## 🔍 `Searcher.hpp` 搜索流程

1. **初始化搜索器**：调用 `BuildIndex` 方法建立索引。
2. **分词**：使用 `cppJieba` 对查询字符串分词。
3. **搜索索引**：遍历查询关键词，获取倒排链表。
4. **结果排序**：按权重降序排序。
5. **生成 JSON**：封装 `title`、`description`、`url` 并返回 JSON。

---

## 📂 重要文件路径
- 📌 **原始 HTML 数据**：`data/input/html`
- 📌 **筛选后的数据**：`data/raw_html`
- 📌 **前端页面**：`wwwroot/index.html`

---

## 🚀 运行步骤
```bash
cd c-_Boost_Search
./http_server
```
然后打开浏览器访问：`http://127.0.0.1:8081`

---

## 🤝 贡献指南
🚀 欢迎贡献代码、提交 Issue 或建议优化！

📜 本项目遵循 **MIT 许可证**。



