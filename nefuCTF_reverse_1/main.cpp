#include "stdafx.h"

#include "crypto.h"

#include <cassert>
#include <random>
#include <set>
#include <string>

int main(int argc, char *argv[]) {
    useCustomKey = true;
    CustomKey    = "In God We Trust.";
    assert(CustomKey.length() == CryptoPP::CIPHER::DEFAULT_KEYLENGTH && "Invalid Key Length.");

#ifdef _DEBUG
    std::string flag = "flag{do_or_do_not_there_is_no_try}";
    hexdumpStr(encryptStr(flag));
    std::cout << std::endl;

    goto show_flag;

    exit(0);
#endif // _DEBUG

#ifndef _DEBUG
    if (IsDebuggerPresent()) {
        std::cerr << "Hey! No debugger allowed here." << std::endl;
        exit(1);
    }
#endif // !_DEBUG

    {
        std::mt19937  mt(std::random_device{}());
        auto          dice_rand   = std::bind(std::uniform_int_distribution<int>(1, 6), mt);
        std::set<int> valid_input = {1, 2, 3, 4, 5, 6};

        volatile int point = dice_rand();
        int          offer = 0;

        std::cout << "I promise to never cheat against you,\n"
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
    }

show_flag:
    byte        test[] = {0xD0, 0xAB, 0xE, 0xF6, 0x29, 0x74, 0x47, 0x19, 0xAA, 0x92, 0x9, 0xE, 0xA3, 0x19, 0xCA, 0x70, 0x25, 0xB9, 0x9A, 0xA7, 0xBC, 0x30, 0x28, 0x5C, 0xEA, 0x60, 0x1D, 0x65, 0x2D, 0x9D, 0x57, 0xCE, 0xB2, 0xE0, 0x78, 0x2C, 0xFE, 0x86, 0x9C, 0x1A, 0x0};
    std::string str(reinterpret_cast<char *>(test));
    std::cout << "So lucky you are! :)" << '\n';
    std::cout << decryptStr(str) << std::endl;

    return 0;
}