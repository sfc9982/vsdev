#include <iostream>
#include <map>
#include <random>
#include <string>

constexpr auto RATIO = 1000;

int main() {
    std::random_device                 rd;
    std::map<int, int>                 hist;
    std::uniform_int_distribution<int> dist(0, 9);

    for (int n = 0; n != 200 * RATIO; ++n)
        ++hist[dist(rd)];
    /* 
       note: demo only: the performance of many implementations 
       of random_device degrades sharply once the entropy pool
       is exhausted. For practical use random_device is generally
       only used to seed a PRNG such as mt19937
    */

    for (auto [x, y] : hist)
        std::cout << " " << x << " : " << std::string(y / RATIO, '*') << '\n';
}