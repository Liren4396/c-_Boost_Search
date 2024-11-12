//index.hpp
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <mutex>
#include "util.hpp"
#include "log.hpp"
namespace ns_index {
    struct DocInfo  {
        std::string title;
        std::string content;
        std::string url;
        uint64_t doc_id;
    };
    struct InvertedElem {
        uint64_t doc_id;
        std::string word;
        int weight; 
    };
    typedef std::vector<InvertedElem> InvertedLIst_t;
    class Index {
        private:
            // positive index uses array and doc'id is unique index
            std::vector<DocInfo> forward_index;
            // inverted index must be a key word relatives to a array of invertedElem
            std::unordered_map<std::string, InvertedLIst_t> inverted_index;
        private:
            Index(){}
            Index(const Index&) = delete;
            Index& operator=(const Index&) = delete;
            static Index* instance;
            static std::mutex mtx;
        public:
            ~Index(){};
        public:
            static Index* GetInstance() {
                if (nullptr == instance) {
                    mtx.lock();
                    if (nullptr == instance) {
                        instance = new Index();
                    }
                    mtx.unlock();
                }
                return instance;
            }
            // use doc_id to find doc content
            DocInfo *GetForwardIndex(const uint64_t &doc_id) {
                if (doc_id > forward_index.size()) {
                    std::cerr << "doc_id out range: error!" << std::endl;
                    return nullptr;
                }
                return &forward_index[doc_id];
            }
            // use key word to find invertedlist
            InvertedLIst_t *GetInvertedList(const std::string &word) {
                auto iter = inverted_index.find(word);
                if (iter == inverted_index.end()) {
                    std::cerr << word << " have no InvertedList!" << std::endl;
                    return nullptr;
                }
                return &(iter->second);
            }
            // 
            // const std::string output = "data/raw_html/raw.txt";
            bool BuildIndex(const std::string &input) { // give the parsed data
                std::ifstream in(input, std::ios::in | std::ios::binary);
                if (!in.is_open()) {
                    std::cerr << "sorry, " << input << " open error " << std::endl;
                    return false;
                }

                std::string line;
                int count = 0;
                while (std::getline(in, line)) {
                    DocInfo* doc =  BuildForwardIndex(line);
                    if (doc == nullptr) {
                        std::cerr << "build: " << line << " error" << std::endl;
                        continue;
                    }
                    BuildInvertedIndex(*doc);
                    count++;
                    if (count % 50 == 0) {
                        //std::cout << "currently already established doc: " << count << std::endl; 
                        LOG(LOG_NORMAL, "currently already established doc: "+std::to_string(count));
                    }
                }
                return true;
            }
        private: 
            DocInfo *BuildForwardIndex(const std::string &line) {
                //1. parse line, split string
                //line -> 3 string, title, content, url.
                
                std::vector<std::string> results;
                std::string sep = "\3";
                ns_util::StringUtil::Split(line, &results, sep);
                if (results.size() != 3) {
                    return nullptr;
                }
                //2. put parsed string into DocInfo
                DocInfo doc;
                doc.title = results[0]; //title
                doc.content = results[1]; // content
                doc.url = results[2];
                doc.doc_id = forward_index.size();
                forward_index.push_back(std::move(doc));
                //3. insert to Forward index vector
                
                return &forward_index.back();

            }

            bool BuildInvertedIndex(const DocInfo &doc) {
                // DocInfo {title, content, url, doc_id}
                // word -> inverted index list
                struct word_cnt {
                    int title_cnt;
                    int content_cnt;
                    word_cnt():title_cnt(0), content_cnt(0) {}
                };
                std::unordered_map<std::string, word_cnt> word_map; // used to save word_cnt temperaly
                std::vector<std::string> title_words;
                ns_util::JiebaUtil::CutString(doc.title, &title_words);
                /*
                if (doc.doc_id == 1572) {
                    for (auto &s : title_words) {
                        std::cout << "title: " << s << std::endl;
                    }
                }*/
                
                for (std::string &s : title_words) {
                    boost::to_lower(s);
                    word_map[s].title_cnt++;
                }
                std::vector<std::string> content_words;
                ns_util::JiebaUtil::CutString(doc.content, &content_words);
                /*
                if (doc.doc_id == 1572) {
                    for (auto &s : content_words) {
                        std::cout << "content: " << s << std::endl;
                    }
                }*/
                for (auto &s : content_words) {
                    boost::to_lower(s);
                    word_map[s].content_cnt++;
                }
                const int kTitleWeight = 10;
                const int kContentWeight = 1;
                for (auto &word_pair : word_map) {
                    InvertedElem item;
                    item.doc_id = doc.doc_id;
                    item.word = word_pair.first;
                    item.weight = kTitleWeight*word_pair.second.title_cnt + kContentWeight*word_pair.second.content_cnt;
                    InvertedLIst_t &inverted_list = inverted_index[word_pair.first];
                    inverted_list.push_back(item);
                }
                return true;
            }
    };
    Index* Index::instance = nullptr;
    std::mutex Index::mtx;
}