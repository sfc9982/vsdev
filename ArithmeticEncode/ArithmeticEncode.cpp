#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <boost/multiprecision/cpp_bin_float.hpp>

using mp_float = boost::multiprecision::cpp_bin_float_100;

auto initialize_probs(const std::string &s) -> std::vector<mp_float>;
auto arithmetic_encode(const std::string &s, const std::vector<mp_float> &probs, const std::vector<mp_float> &probs_sum) -> mp_float;
auto arithmetic_decode(const mp_float &encoded, const std::string &s, const std::vector<mp_float> &probs, const std::vector<mp_float> &probs_sum) -> std::string;

auto main() -> int {
    const std::string     s     = "INGODWETRUST";
    auto                  probs = initialize_probs(s);
    std::vector<mp_float> probs_sum(26, 0.f);

    std::partial_sum(probs.begin(), probs.end(), probs_sum.begin());

    const mp_float    encoded = arithmetic_encode(s, probs, probs_sum);
    const std::string decoded = arithmetic_decode(encoded, s, probs, probs_sum);

    std::cout << std::setprecision(std::numeric_limits<mp_float>::max_digits10);

    std::cout << "Encoded value: " << encoded << std::endl;
    std::cout << "Decoded string: " << decoded << std::endl;

    return 0;
}

auto initialize_probs(const std::string &s) -> std::vector<mp_float> {
    const size_t          total_chars = s.length();
    std::vector<int>      char_count(26, 0);
    std::vector<mp_float> probs(26, 0.f);

    for (const char c : s) {
        char_count[c - 'A']++;
    }
    for (int i = 0; i < 26; i++) {
        probs[i] = static_cast<mp_float>(char_count[i]) / total_chars;
    }

    return probs;
}

auto arithmetic_encode(const std::string &s, const std::vector<mp_float> &probs, const std::vector<mp_float> &probs_sum) -> mp_float {
    mp_float start = 0.0;
    mp_float end   = 1.0;

    for (const char c : s) {
        const int index = c - 'A';
        mp_float  range = end - start;

        start = start + range * (index == 0 ? 0 : probs_sum[index - 1]);
        end   = start + range * probs[index];
    }

    return start;
}

auto arithmetic_decode(const mp_float &encoded, const std::string &s, const std::vector<mp_float> &probs, const std::vector<mp_float> &probs_sum) -> std::string {
    std::string decoded;
    mp_float    start = 0.0;
    mp_float    end   = 1.0;

    for (std::size_t i = 0; i < s.length(); i++) {
        int      index = 0;
        mp_float range = end - start;
        while (encoded >= start + range * probs_sum[index]) {
            index++;
        }
        decoded += static_cast<char>('A' + index);
        start = start + range * (index == 0 ? 0 : probs_sum[index - 1]);
        end   = start + range * probs[index];
    }

    return decoded;
}