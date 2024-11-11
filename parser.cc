//parser.cc
#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include "util.hpp"
//Dir, where all html files putted
const std::string src_path = "data/input/html";
const std::string output = "data/raw_html/raw.txt";

typedef struct DocInfo {
    std::string title;   // title
    std::string content; // content  
    std::string url;     // url
} DocInfo_t;

// const & : input
// *: output
// &: input output
bool EnumFile(const std::string &src_path, std::vector<std::string> *files_list);
bool ParseHtml(const std::vector<std::string> &files_list, std::vector<DocInfo_t> *results);
bool SaveHtml(const std::vector<DocInfo_t> &results, const std::string &output);


int main()
{
    std::vector<std::string> file_list;
    // iterate each html file name and save to file_list
    if (!EnumFile(src_path, &file_list)) {
        std::cerr << "enum file name error!" << std::endl;
        return 1;
    }

    // 2 : parse file_list's file content to results
    std::vector<DocInfo_t> results;
    if (!ParseHtml(file_list, &results)) {
        std::cerr << "parse html error" << std::endl;
        return 2;
    }

    // 3: put results to output. let \3 be the delemitor to each document.
    if (!SaveHtml(results, output)) {
        std::cerr << "save html error" << std::endl;
        return 3;
    }
    return 0;
}

bool EnumFile(const std::string &src_path, std::vector<std::string> *files_list) {
    namespace fs =  boost::filesystem;
    fs::path root_path(src_path);
    // if path not exist, return
    if (!fs::exists(root_path)) {
        std::cerr << src_path << " not exists" << std::endl;
        return false;
    }
    // define a null iteration to distinguish the iteration end
    fs::recursive_directory_iterator end;
    for (fs::recursive_directory_iterator iter(root_path); iter != end; iter++) {
        // if not regular file, return
        if (!fs::is_regular_file(*iter)) {
            continue;
        }
        // if filename's extension not match html, return
        if (iter->path().extension() != ".html") {
            continue;
        }
        // std::cout << "debug: " << iter->path().string() << std::endl;
        files_list->push_back(iter->path().string());
    }

    return true;
}

static bool ParseTitle(const std::string &file, std::string* title) {
    std::size_t begin = file.find("<title>");
    if (begin == std::string::npos) {
        return false;
    }
    std::size_t end = file.find("</title>");
    if (end == std::string::npos) {
        return false;
    }
    begin += std::string("<title>").size();
    if (begin > end) {
        return false;
    }
    *title = file.substr(begin, end-begin);
    return true;
}

static bool ParseContent(const std::string &file, std::string* content) {
    enum status{
        LABLE,
        CONTENT
    };
    enum status s = LABLE;
    for (char c : file) {
        switch ((s))
        {
        case LABLE:
            if (c == '>') {
                s = CONTENT;
            }
            break;
        case CONTENT:
            if (c == '<') {
                s = LABLE;
            } else {
                if (c == '\n') {
                    c = ' ';
                }
                content->push_back(c);
            }
            break;
        default:
            break;

        }
    }
    return true;
}

// url head : www.boost.org/doc/libs/1_85_0/doc/html
// url tail : filename
static bool ParseUrl(const std::string &file_path, std::string *url) {
    std::string url_head = "https://www.boost.org/doc/libs/1_85_0/doc/html";
    std::string url_tail = file_path.substr(src_path.size());
    *url = url_head + url_tail;
    return true;
}

static void ShowDoc(const DocInfo_t &doc) {
    std::cout << "title: " << doc.title << std::endl;
    std::cout << "content: " << doc.content << std::endl;
    std::cout << "url: " << doc.url << std::endl;
}

bool ParseHtml(const std::vector<std::string> &files_list, std::vector<DocInfo_t> *results) {
    for (const std::string &file : files_list) {
        std::string result;
        // read file
        // parse file to extract title, content and url
        if (!ns_util::FileUtil::ReadFile(file, &result)) {
            continue;
        }
        DocInfo_t doc;

        if (!ParseTitle(result, &doc.title)) {
            continue;
        }
        if (!ParseContent(result, &doc.content)) {
            continue;
        }
        if (!ParseUrl(file, &doc.url)) {
            continue;
        }
        // done finish parse, all documents relative results are saved in doc
        results->push_back(std::move(doc));  // bug: copy and paste
        // for debug
        //ShowDoc(doc);
    }
    return true;
}
bool SaveHtml(const std::vector<DocInfo_t> &results, const std::string &output) {
#define SEP '\3'
    std::ofstream out(output, std::ios::out | std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "open " << output << " failed!" << std::endl;
        return false;
    }
    for (auto &item: results) {
        std::string out_string;
        out_string = item.title;
        out_string += SEP;
        out_string += item.content;
        out_string += SEP;
        out_string += item.url;
        out_string += '\n';

        out.write(out_string.c_str(), out_string.size());
    }
    out.close();

    return true;
}
