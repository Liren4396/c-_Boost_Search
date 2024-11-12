//log.hpp
#pragma once
#include <iostream>
#include <string>
#include <ctime>

#define LOG_NORMAL  1
#define LOG_WARNING 2
#define LOG_DEBUG    3
#define LOG_FATAL   4

#define LOG(LEVEL, MESSAGE) log(#LEVEL, MESSAGE, __FILE__, __LINE__)

void log(std::string level, std::string message, std::string file, int line) {
    std::cout << "[" << level << "]" << "[" << message << "]" << "[" << file << ":" << line << "]" << std::endl;
}