#include <boost/multiprecision/cpp_dec_float.hpp>
#include <iomanip>
#include <iostream>
#include <random>

#include "vector.hpp"

#include "ERK.hpp"

namespace mp = boost::multiprecision;

float_mp angular_momentum(mino2357::vector<float_mp> &x) {
    auto L   = float_mp("0.0");
    auto num = x.vec.size();
    for (size_t i = 0; i < num / 2; i += 2) {
        L += x.vec[i] * x.vec[i + num / 2 + 1] - x.vec[i + 1] * x.vec[i + num / 2];
    }
    return L;
}

int main() {
    //std::cout << std::fixed << std::setprecision(std::numeric_limits<float_mp>::digits10 + 1);
    std::cerr << std::fixed << std::setprecision(50);
    std::cout << std::fixed << std::setprecision(std::numeric_limits<double>::digits10 + 1);

    size_t                     n   = 32;
    size_t                     num = 4 * n; // 3[n]*2[dim]*2[pos, vel]
    mino2357::vector<float_mp> x(num);

    unsigned int                     seed1 = 0;
    std::mt19937                     mt(seed1);
    std::uniform_real_distribution<> rand01(-1.0, 1.0);

    for (size_t i = 0; i < num / 2; i++) {
        x.vec[i] = static_cast<float_mp>(rand01(mt));
    }

    auto vel = float_mp("0.0");
    for (size_t i = num / 2; i < num; i += 2) {
        //std::cout << i << " " << i + 1 << " " << i - num / 2 << " " << i - num / 2 + 1 << std::endl;
        x.vec[i]     = -vel * x.vec[i - num / 2];
        x.vec[i + 1] = vel * x.vec[i - num / 2 + 1];
    }

    auto                      tol        = float_mp("1.0e-9");
    auto                      dt         = float_mp("1.0e-12");
    auto                      intv       = float_mp("1.0e-5");
    auto                      write_time = float_mp("0.1") * mino2357::min_dt<float_mp>();
    auto                      time       = float_mp("0.0");
    auto                      end_time   = float_mp("1.0e2");
    mino2357::RKF78<float_mp> rkf78(tol, tol);

    for (;;) {
        rkf78.Integrate(time, dt, x);
        if (time > write_time) {
            std::cerr << time << " " << angular_momentum(x) << std::endl;

            for (size_t i = 0; i < num / 2; i++) {
                std::cout << x.vec[i] << " ";
            }
            std::cout << std::endl;
            write_time = time + intv;
        }
        if (time > end_time) {
            break;
        }
    }
}