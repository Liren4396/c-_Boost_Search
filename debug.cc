// debug.cc
#include "searcher.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
const std::string input = "data/raw_html/raw.txt";

int main() {
    ns_searcher::Searcher *search = new ns_searcher::Searcher();
    search->InitSearcher(input);
    std::cout << "finish init." << std::endl;
    std::string query;
    std::string json_string;
    Saved_data::Saved_datas* pb_result = new Saved_data::Saved_datas();
    std::string serialized_data;
    
    char buffer[1024];
    while (true) {
        std::cout << "Please Enter You Search Query# ";
        //std::cin >> query;
        fgets(buffer, sizeof(buffer)-1, stdin);
        buffer[strlen(buffer)-1] = 0;
        query = buffer;
        search->Search(query, pb_result);
        pb_result->SerializeToString(&serialized_data);
        //std::cout << query << std::endl;
        std::cout << serialized_data << std::endl;
    }
    return 0;
}