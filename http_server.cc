//http_server.cc
#include "searcher.hpp"
#include "cpp-httplib/httplib.h"
#include <google/protobuf/util/json_util.h>
const std::string root_path = "./wwwroot";
const std::string input = "data/raw_html/raw.txt";
int main() {
    httplib::Server svr;
    ns_searcher::Searcher search;
    search.InitSearcher(input);
    
    svr.set_base_dir(root_path.c_str());
    svr.Get("/saved_data.proto", [](const httplib::Request &req, httplib::Response &rsp){
        std::ifstream proto_file("./saved_data.proto", std::ios::in | std::ios::binary);
        if (proto_file.is_open()) {
            std::stringstream buffer;
            buffer << proto_file.rdbuf();
            rsp.set_content(buffer.str(), "text/plain");
        } else {
            rsp.status = 404;
            rsp.set_content("File not found", "text/plain");
        }
    });
    svr.Get("/s", [&search](const httplib::Request &req, httplib::Response &rsp){
        //rsp.set_content("hello world!", "text/plain; charset=utf-8");
        if (!req.has_param("word")) {
            rsp.set_content("must have key word to search", "text/plain: charset=utf-8");
            return;
        }
        std::string word = req.get_param_value("word");
        LOG(LOG_NORMAL, "user is searching: "+word);
        //std::cout << "user is searching: " << word << std::endl;
        /*
        std::string json_string;
        search.Search(word, &json_string);
        rsp.set_content(json_string, "application/json");
        */
        Saved_data::Saved_datas* pb_result = new Saved_data::Saved_datas();
        search.Search(word, pb_result);
        std::string serialized_data;
        pb_result->SerializeToString(&serialized_data);
        rsp.set_header("Content-Type", "application/x-protobuf");
        rsp.set_content(serialized_data, "application/x-protobuf");

    });
    LOG(NORMAL, "run server successfully");
    svr.listen("0.0.0.0", 8081);
    return 0;
}