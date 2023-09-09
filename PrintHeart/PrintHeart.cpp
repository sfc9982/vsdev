#include <cmath>
#include <iostream>

int main() {
    // 设置绘制范围
    const int size = 10;

    // 绘制心形
    for (int y = size; y >= -size; --y) {
        for (int x = -2 * size; x <= 2 * size; ++x) {
            double dist1 = std::sqrt(std::pow(x / static_cast<double>(size), 2) + std::pow(y / static_cast<double>(size), 2));
            double dist2 = std::sqrt(std::pow((x - 1) / static_cast<double>(size), 2) + std::pow((y - 1) / static_cast<double>(size), 2));
            if (dist1 < 1.6 && std::abs(dist2 - 1) < 0.4) {
                std::cout << "* ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}