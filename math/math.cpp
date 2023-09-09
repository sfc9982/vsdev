#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

using std::cin, std::cout, std::endl;
using namespace boost::multiprecision;

cpp_int a[] = {5, 7, 8};

int main() {
    cpp_int n, ans = 0;
    cin >> n;

    ans = ((n - 2) / 3) * 4 + a[((n - 2) % 3).convert_to<size_t>()];

    cout << ans << endl;

    return 0;
}