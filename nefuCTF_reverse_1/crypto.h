#pragma once

#include <string>

void        hexdumpStr(std::string str);
std::string encryptStr(std::string str);
std::string decryptStr(std::string str);

extern bool        useCustomKey;
extern std::string CustomKey;
