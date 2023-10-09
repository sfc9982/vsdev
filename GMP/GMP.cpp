#include <iostream>

#include <gmpxx.h>

using gz = mpz_class;

int main() {
    mpz_class x, y, z;

    std::cin >> x >> y;

    mpz_ui_pow_ui(z.get_mpz_t(), x.get_ui(), y.get_ui());

    std::cout << z << std::endl;
}
