//util.hpp
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "data/cppjieba/include/cppjieba/Jieba.hpp"

const char* const DICT_PATH = "data/cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "data/cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "data/cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "data/cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "data/cppjieba/dict/stop_words.utf8";
namespace ns_util {
    class FileUtil {
        public:
            static bool ReadFile(const std::string &file_path, std::string *out) {
                
                std::ifstream in(file_path, std::ios::in);
                if (!in.is_open()) {
                    std::cerr << "open file" << file_path << " error" << std::endl;
                    return false;
                }

                std::string line;
                while (std::getline(in, line)) {
                    *out += line;
                }
                in.close();
                return true;
            }
    };
    class StringUtil{
        public:
            static void Split(const std::string &target, std::vector<std::string> *out, std::string &sep) {
                // boost split
                // aaa\3bbb\3\3
                boost::split(*out, target, boost::is_any_of(sep), boost::token_compress_on);
                
            }
    };
    class JiebaUtil {
        private:
            static cppjieba::Jieba jieba;
        public:
            static void CutString(const std::string &src, std::vector<std::string> *out) {
                jieba.CutForSearch(src, *out);
            }
    };
    cppjieba::Jieba JiebaUtil::jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);
}