# [CppReference 搜索引擎]
*项目背景*
站内搜索: 搜索的数据更垂直，数据量更小

*技术栈和项目环境*
技术栈: C/C++ C++11, STL, 准标准库Boost， jsonCpp, cppjieba,
项目环境 Ubuntu 22.04 g++/Makefile vscode

*项目原理*
服务端
1.下载cppReference对应的html文件夹 
2.创建数据筛选 (去标签和数据清理) 的程序
3.建立正向索引和倒排索引保存并加速查找网页相关信息
5.检索索引得到相关html
6.通过拼接多个网页的标题+内容介绍+对应网址，构建全新的网站返回给用户
客户端
4.用户发起对应http请求 get方法 上传搜索 关键字

*功能模块介绍*
*数据筛选* -- parse.cc
第三方库: boost::filesystem (ReadFile 读取文件 split 分割内容) -- 具体实现util.hpp
1. 查找文件:  遍历文件名并保存有效文件名到file_list数组中 -- EnumFile 函数
2. 去标签: 遍历file_list拿到对应内容进行 标题 内容 网址 的筛选，保存到results数组中 -- ParseTitle 函数
3. 拼接: 遍历results 并以\3为分隔符分割不同的文件 拼接results数组中所有内容

*索引* -- index.hpp
第三方库: cppJieba (CutForSearch 函数) 实现中文分词  -- 具体实现util.hpp
1.索引管理器: 单例模式实现, 保存正向索引和倒排索引 
数据结构
DocInfo : title, content, url, doc_id
InvertedElem: doc_id, word, weight(权重)
2.正向索引: 
数据结构: vector 
  下标: id
  保存的内容: DocInfo
3.倒排索引:
数据结构: unordered_map
  key: 关键字 string
  value: vector<InvertedElem>

*服务器构建* -- http_server.cc
第三方库: httplib, jsonCpp
功能: 接收用户的搜索请求，调用Searcher类进行搜索操作，并将搜索结果以 JSON 格式返回给客户端。
Searcher.hpp 流程介绍
1. 初始化搜索器 获取或创建一个索引对象实例，然后调用索引对象的BuildIndex方法建立正向和倒排索引。
2. 分词：根据用户输入的查询字符串进行分词，将结果存储在words向量中。
3. 索引搜索：遍历分词后的每个关键词，通过索引对象获取对应的倒排链表。
   将搜索结果合并到inverted_list_all向量中，该向量存储了InvertedElemPrint结构体，包含文档 ID、权重和关键词列表。
4. 排序：对合并后的搜索结果按照权重进行降序排序。
5. 生成 JSON：遍历排序后的搜索结果，通过索引对象获取文档信息，将文档的标题、描述、URL 等信息封装成 JSON 对象，并添加到root对象中。
   最后使用Json::StyledWriter将root对象转换为 JSON 格式的字符串。

必要文件路径:
前端页面: wwwroot/index.html
cppReference的基本内容: data/input/html
数据筛选完的路径: data/raw_html
