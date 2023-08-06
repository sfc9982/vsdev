#include "stdafx.h"

#include "crypto.h"

#include <random>
#include <set>

int main(int argc, char *argv[]) {
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

    byte        test[] = {0x50, 0x22, 0xA9, 0x5A, 0x8F, 0xB, 0x50, 0x56, 0x19, 0xC8, 0xB7, 0x3, 0xA, 0x6C, 0x3, 0x70, 0x45, 0x6F, 0x15, 0x4C, 0x39, 0x2B, 0xD2, 0x2C, 0x0};
    std::string str(reinterpret_cast<char *>(test));
    std::cout << "So lucky you are! :)" << '\n';
    std::cout << decryptStr(str) << std::endl;

    return 0;
}