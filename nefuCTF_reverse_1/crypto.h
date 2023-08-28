#pragma once

#include <string>

void        hexdumpStr(std::string str);
std::string encryptStr(const std::string &str);
std::string decryptStr(const std::string &str);

extern bool        useCustomKey;
extern std::string CustomKey;
