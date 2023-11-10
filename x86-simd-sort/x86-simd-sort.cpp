#include <cstdio>

#include "src/avx512-32bit-qsort.hpp"

int main() {	
    const int ARRSIZE = 1000;
    std::vector<float> arr;

    /* Initialize elements is reverse order */
    for (int ii = 0; ii < ARRSIZE; ++ii) {
        arr.push_back(ARRSIZE - ii);
    }

    for (int ii = 0; ii < ARRSIZE; ++ii) {
        printf("%.0f\n", arr[ii]);
    }
    
    system("pause");

    /* call avx512 quicksort */
    avx512_qsort(arr.data(), ARRSIZE);
    
    for (int ii = 0; ii < ARRSIZE; ++ii) {
        printf("%.0f\n", arr[ii]);
    }
    
    return 0;
}
