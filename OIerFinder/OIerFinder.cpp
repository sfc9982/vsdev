#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include <algorithm>
#include <fast-cpp-csv-parser/csv.h>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <json/json.h>


constexpr unsigned int CSV_COLUMN = 9;

std::string sliceName(std::string);

int main() {
    io::CSVReader<CSV_COLUMN> in("oierdb.csv");

    int         uid;
    std::string initials;
    std::string name;
    int         gender;
    int         enroll_middle;
    double      oierdb_score;
    double      ccf_score;
    int         ccf_level;
    std::string compressed_records;

    std::set<std::string> oiers;

    auto start = std::chrono::steady_clock::now();
    while (in.read_row(uid, initials, name, gender, enroll_middle, oierdb_score, ccf_score, ccf_level, compressed_records)) {
        oiers.insert(name);
    }
    auto end     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    fmt::println("{} records loaded.", oiers.size());
    fmt::println("Time cost: {}", elapsed);


    Json::Value   newbie;
    std::ifstream newbie_file("newbie.json", std::ifstream::binary);
    newbie_file >> newbie;

    const Json::Value        data = newbie["data"];
    std::vector<std::string> nickVec;
    for (const auto &index : data) {
        if (!index["card"].empty()) {
            std::string card = index["card"].asString();
            std::string name = sliceName(card);
            // fmt::println(name);
            nickVec.emplace_back(name);
        }
    }

    for (const auto &nefuer : nickVec) {
        if (oiers.count(nefuer) > 0) {
            fmt::println(nefuer);
        }
    }

    return 0;
}

std::string sliceName(std::string name) {
    // How could they typed so many Unicode dashes???
    name            = std::regex_replace(name, std::regex("–|—|－|—|人工智能|大数据|计类|计算机类|计算机"), "-");
    std::size_t pos = std::string::npos;

    // TODO: wrong way, should not written like this
    for (size_t i = name.length(); i > 0; --i) {
        if (name[i] > 0 || name[i] == '-') {
            pos = i;
            break;
        }
    }

    if (name[0] > 0 || name[0] == '-') {
        pos = std::max(0, static_cast<int>(pos));
    }

    if (pos == std::string::npos) {
        return name;
    }

    std::string slice = name.substr(pos + 1);

    return slice;
}