#include <algorithm>
#include <chrono>
#include <iostream>

constexpr size_t RANGESZ = 10000;

int main() {
    const unsigned arraySize = 32768;
    int            data[arraySize];

    for (int &c : data) {
        c = std::rand() % 256;
    }

    // std::sort(data, data + arraySize);

    long long sum = 0;

    auto start = std::chrono::steady_clock::now();

    for (unsigned i = 0; i < RANGESZ; ++i) {
        for (unsigned c = 0; c < arraySize; ++c) {
            if (data[c] >= 128)
                sum += data[c];
        }
    }

    auto end     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "time = " << elapsed.count() << "ms" << std::endl;
    std::cout << "sum  = " << sum << std::endl;
}