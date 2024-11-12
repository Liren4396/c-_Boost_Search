//test.cc
// jsoncpp test
/*
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

    // std::vector<std::string> result;
    // std::string target = "aaaa\3\3\3bbbb\3ccccc";
    // boost::split(result, target, boost::is_any_of("\3"), boost::token_compress_on);
    // for (auto & s : result) {
    //     std::cout << s << std::endl;
    // }
}*/

#include "saved_data.pb.h"
#include <iostream>

int main() {
    Saved_data::Saved_datas* ret = new Saved_data::Saved_datas();
    Saved_data::Saved_datas datas;
    Saved_data::Saved_data data;
    data.set_title("nihao");
    data.set_desc("hah");
    data.set_url("url");
    data.set_id(1);
    int weight_value = 123; // 假设这是原本的整数权重值
    data.set_weight(weight_value);
    std::cout << "Title: " << data.title() << std::endl;
    std::cout << "Description: " << data.desc() << std::endl;
    std::cout << "URL: " << data.url() << std::endl;
    std::cout << "ID: " << data.id() << std::endl;
    std::cout << "Weight: " << data.weight() << std::endl;
    datas.add_data()->CopyFrom(data);
    std::string serialized_data;
    datas.SerializeToString(&serialized_data);
    
    *ret = datas;
    std::cout << serialized_data << std::endl;

}