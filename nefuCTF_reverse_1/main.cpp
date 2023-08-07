#include "stdafx.h"

#include "crypto.h"

#include <random>
#include <set>

int main(int argc, char *argv[]) {
#ifdef _DEBUG
    std::string flag = "flag{do_or_do_not_there_is_no_try}";
    hexdumpStr(encryptStr(flag));
#endif // _DEBUG

#ifndef _DEBUG
    if (IsDebuggerPresent()) {
        std::cerr << "Hey! No debugger allowed here." << std::endl;
        exit(1);
    }
#endif // !_DEBUG

    std::mt19937  mt(std::random_device{}());
    auto          dice_rand   = std::bind(std::uniform_int_distribution<int>(1, 6), mt);
    std::set<int> valid_input = {1, 2, 3, 4, 5, 6};

    volatile int point = dice_rand();
    int          offer = 0;

    std::cout << "I promise to never cheat against you,"
                 "\n"
                 "please give me your offer (1-6): ";
    std::cin >> offer;
    if (!valid_input.count(offer)) {
        std::cerr << "Invalid input!" << std::endl;
        exit(1);
    }

    while (offer == point) {
        point = dice_rand();
    }

    if (offer != point) {
        std::cerr << "Wrong guess!" << std::endl;
        exit(1);
    }

    byte        test[] = {0x3C, 0x69, 0x3C, 0xCE, 0x2B, 0xA0, 0x1B, 0xB8, 0x3, 0xA4, 0xF2, 0x6E, 0xC2, 0x41, 0x61, 0xB7, 0xED, 0x2C, 0xA0, 0xEA, 0xB6, 0x11, 0xEC, 0x6, 0x29, 0xB2, 0xCC, 0x43, 0x3C, 0x60, 0xFF, 0x4D, 0x1C, 0x77, 0x57, 0x3A, 0xDE, 0xD6, 0x6F, 0xD8};
    std::string str(reinterpret_cast<char *>(test));
    std::cout << "So lucky you are! :)" << '\n';
    std::cout << decryptStr(str) << std::endl;

    return 0;
}