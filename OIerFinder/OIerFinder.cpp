#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include <fast-cpp-csv-parser/csv.h>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <json/json.h>
#include <unicode/regex.h>
#include <unicode/unistr.h>

constexpr unsigned int CSV_COLUMN   = 9;
constexpr int          CURRENT_YEAR = 2023;

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
        if (enroll_middle + 6 == CURRENT_YEAR) {
            oiers.insert(name);
        }
    }
    auto end     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    fmt::println("{} records loaded.", oiers.size());
    fmt::println("Time cost: {}", elapsed);
    fmt::println("\nOIers: ");


    Json::Value   newbie;
    std::ifstream newbie_file("newbie.json", std::ifstream::binary);
    newbie_file >> newbie;

    const Json::Value        data = newbie["data"];
    std::vector<std::string> nickVec;
    for (const auto &index : data) {
        if (!index["card"].empty()) {
            std::string cardName = index["card"].asString();
            std::string name     = sliceName(cardName);
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
    UErrorCode         status = U_ZERO_ERROR;
    icu::UnicodeString ucs    = icu::UnicodeString::fromUTF8(icu::StringPiece(name.c_str()));
    icu::RegexMatcher  matcher("^.*( |-|–|—|－|人工智能|大数据|类|计算机)", 0, status);
    if (U_FAILURE(status)) {
        fmt::println("Regex init failed.");
    }

    matcher.reset(ucs);
    icu::UnicodeString result;
    icu::UnicodeString replacement("");

    while (matcher.find(status) && U_SUCCESS(status)) {
        matcher.appendReplacement(result, replacement, status);
    }
    matcher.appendTail(result);

    char result_buf[100];
    result.extract(0, result.length(), result_buf, sizeof(result_buf));

    return {result_buf};
}