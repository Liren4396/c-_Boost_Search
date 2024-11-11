//http_server.cc
#include "searcher.hpp"
#include "cpp-httplib/httplib.h"
const std::string root_path = "./wwwroot";
const std::string input = "data/raw_html/raw.txt";
int main() {
    httplib::Server svr;
    ns_searcher::Searcher search;
    search.InitSearcher(input);
    
    svr.set_base_dir(root_path.c_str());
    svr.Get("/s", [&search](const httplib::Request &req, httplib::Response &rsp){
        //rsp.set_content("hello world!", "text/plain; charset=utf-8");
        if (!req.has_param("word")) {
            rsp.set_content("must have key word to search", "text/plain: charset=utf-8");
            return;
        }
        std::string word = req.get_param_value("word");
        LOG(NORMAL, "user is searching: "+word);
        //std::cout << "user is searching: " << word << std::endl;
        std::string json_string;
        //search.Search(word, &json_string);
        search.Search(word, &json_string);
        rsp.set_content(json_string, "application/json");
    });
    LOG(NORMAL, "run server successfully");
    svr.listen("0.0.0.0", 8081);
    return 0;
}