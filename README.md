# Boost_Searcher
**CppReference 搜索引擎**

**一、项目背景**  
站内搜索，数据更垂直，数据量更小。

**二、技术栈和项目环境**  
技术栈：C/C++（C++11）、STL、准标准库 Boost、jsonCpp、cppjieba。  
项目环境：Ubuntu 22.04、g++/Makefile、VSCode。  

**三、项目原理**  
**（一）服务端**  
下载 CppReference 对应的 html 文件夹。  
创建数据筛选程序（去标签和数据清理）。  
建立正向索引和倒排索引，保存并加速查找网页相关信息。  
检索索引得到相关 html。  
通过拼接多个网页的标题、内容介绍和对应网址，构建全新的网站返回给用户。  
**（二）客户端**  
用户发起对应 HTTP 请求（GET 方法），上传搜索关键字。

**四、功能模块介绍**  
**（一）数据筛选(parse.cc)**
第三方库：boost::filesystem（ReadFile 读取文件、split 分割内容，具体实现于 util.hpp）。  
查找文件：遍历文件名并保存有效文件名到 file_list 数组中（EnumFile 函数）。  
去标签：遍历 file_list 拿到对应内容进行标题、内容、网址的筛选，保存到 results 数组中（ParseTitle 函数）。  
拼接：遍历 results 并以 '\3' 为分隔符分割不同的文件，拼接 results 数组中所有内容。  
**（二）索引（index.hpp）**  
第三方库：cppJieba（CutForSearch 函数实现中文分词，具体实现于 util.hpp）。  
索引管理器：单例模式实现，保存正向索引和倒排索引。  
数据结构：  
DocInfo：包含 title、content、url、doc_id。  
InvertedElem：包含 doc_id、word、weight（权重）。  
正向索引：  
数据结构为 vector，下标为 id，保存的内容为 DocInfo。  
倒排索引：  
数据结构为 unordered_map，key 为关键字 string，value 为 vector<InvertedElem>。  
**（三）服务器构建（http_server.cc）**  
第三方库：httplib、jsonCpp。  
功能：接收用户的搜索请求，调用 Searcher 类进行搜索操作，并将搜索结果以 JSON 格式返回给客户端。

**五、Searcher.hpp 流程介绍**  
初始化搜索器：获取或创建一个索引对象实例，然后调用索引对象的 BuildIndex 方法建立正向和倒排索引。  
分词：根据用户输入的查询字符串进行分词，将结果存储在 words 向量中。  
索引搜索：遍历分词后的每个关键词，通过索引对象获取对应的倒排链表。将搜索结果合并到 inverted_list_all 向量中，该向量存储了 InvertedElemPrint 结构体，包含文档 ID、权重和关键词列表。  
排序：对合并后的搜索结果按照权重进行降序排序。  
生成 JSON：遍历排序后的搜索结果，通过索引对象获取文档信息，将文档的标题、描述、URL 等信息封装成 JSON 对象，并添加到 root 对象中。最后使用 Json::StyledWriter 将 root 对象转换为 JSON 格式的字符串

**六、必要文件路径**  
前端页面：wwwroot/index.html  
CppReference 的基本内容：data/input/html  
数据筛选完的路径：data/raw_html

