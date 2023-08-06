#include <algorithm>
#include <chrono>
#include <iostream>
#pragma warning(disable : 4235)

int main() {
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < 1e9; ++i) {
        __asm("nop");
    }

    auto end      = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Duration: " << duration.count() << "ms" << std::endl;
}
