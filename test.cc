//test.cc
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <json/json.h>
//Value Reader Writer
int main() {
    Json::Value root;
    Json::Value item1;
    item1["key1"] = "value1";
    item1["key2"] = "value2";

    Json::Value item2;
    item2["key1"] = "value1";
    item2["key2"] = "value2";

    root.append(item1);
    root.append(item2);

    Json::StyledWriter writer;
    std::string s = writer.write(root);
    std::cout << s << std::endl;
    /*
    std::vector<std::string> result;
    std::string target = "aaaa\3\3\3bbbb\3ccccc";
    boost::split(result, target, boost::is_any_of("\3"), boost::token_compress_on);
    for (auto & s : result) {
        std::cout << s << std::endl;
    }*/
}