//searcher.hpp
#include "index.hpp"
#include "util.hpp"
#include <algorithm>
#include <json/json.h>
#include "log.hpp"
namespace ns_searcher {
    struct InvertedElemPrint {
        uint64_t doc_id;
        int weight;
        std::vector<std::string> words;
        InvertedElemPrint():doc_id(0), weight(0) {};
    };
    class Searcher{
        private:
            ns_index::Index *index;
        public:
            Searcher(){}
            ~Searcher(){}
        public:
            void InitSearcher(const std::string &input) {
                // 1. get or create index obj
                // 2. establish index from index obj
                index = ns_index::Index::GetInstance();
                LOG(NORMAL, "get instance successfully...");
                //std::cout << "get instance successfully..." << std::endl;
                index->BuildIndex(input);
                //std::cout << "establish forward and backward index successfully..." << std::endl;
                LOG(NORMAL, "establish forward and backward index successfully...");
            }
            // query: search key word
            // json_string: result to user's web
            void Search(const std::string &query, std::string *json_string) {
                // 1. split word by following searcher's request
                std::vector<std::string> words;
                ns_util::JiebaUtil::CutString(query, &words);

                // 2. by the splited word to do index search
                //ns_index::InvertedLIst_t inverted_list_all; //internal InvertedElem
                std::vector<InvertedElemPrint> inverted_list_all;

                std::unordered_map<uint64_t, InvertedElemPrint> tokens_map;

                for (std::string word : words) {
                    boost::to_lower(word);
                    ns_index::InvertedLIst_t *inverted_list = index->GetInvertedList(word);
                    if (nullptr == inverted_list) {
                        continue;
                    }
                    //inverted_list_all.insert(inverted_list_all.end(), inverted_list->begin(), inverted_list->end());
                    for (const auto &elem : *inverted_list) {
                        auto &item = tokens_map[elem.doc_id];
                        item.doc_id = elem.doc_id;
                        item.weight += elem.weight;
                        item.words.push_back(elem.word);
                    }
                }
                for (const auto &item : tokens_map) {
                    inverted_list_all.push_back(std::move(item.second));
                }
                // 3. merge sort, using weight to desc or ascending order
                //std::sort(inverted_list_all.begin(), inverted_list_all.end(), \
                //    [](const ns_index::InvertedElem &e1, const ns_index::InvertedElem &e2){
                //       return e1.weight > e2.weight;
                //    });
                std::sort(inverted_list_all.begin(), inverted_list_all.end(), \
                    [](const InvertedElemPrint &e1, const InvertedElemPrint &e2){
                        return e1.weight > e2.weight;
                    });
                // 4. establish json --- jsoncpp --- using jsoncpp series and unseries
                Json::Value root;
                for (auto &item : inverted_list_all) {
                    ns_index::DocInfo * doc = index->GetForwardIndex(item.doc_id);
                    if (nullptr == doc) {
                        continue;
                    }
                    Json::Value elem;
                    elem["title"] = doc->title;
                    elem["desc"] = GetDesc(doc->content, item.words[0]); // content is parsed ret, we only need a part
                    elem["url"] = doc->url;

                    //for debug, for delete
                    elem["id"] = (int)item.doc_id;
                    elem["weight"] = item.weight; // int -> string

                    root.append(elem);
                }
                Json::StyledWriter writer;
                *json_string = writer.write(root);
            }
            std::string GetDesc(const std::string &html_content, const std::string &word) {
                // find the first place html_content appear,
                // forward 50 bytes(if no 50, start from begin), backwars 100 bytes(if no 100, stop at end)
                // 1. find first appearred place
                int prev_step = 50;
                int next_step = 100;
                // word has been changed to lowercase pos 
                // std::size_t pos = html_content.find(word);
                //if (pos == std::string::npos) {
                //    return "None1"; // key word doesnt exist
                //}
                auto iter = std::search(html_content.begin(), html_content.end(), word.begin(), word.end(), [](int x, int y){
                    return (std::tolower(x) == std::tolower(y));
                });
                if (iter == html_content.end()) {
                    return "None1";
                }
                int pos = std::distance(html_content.begin(), iter);
                // 2. get start, end(unsigned integer)
                int start = 0;
                int end = html_content.size()-1;
                // if before has 50+ bytes, updates pos
                // pos - prev_step > start
                if (pos > start + prev_step) {
                    start = pos - prev_step;
                }
                // pos + next_step < end
                if (pos < end - next_step) {
                    end = pos + 100;
                }
                // 3. extract sub string, return
                if (start >= end) return "None2";
                std::string desc = html_content.substr(start, end-start);
                desc += "...";
                return desc;
            }
    };
}