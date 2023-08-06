#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#include <cstdlib>
#include <iostream>

#define MEMLEAK_REPORT                                   \
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);     \
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);   \
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);    \
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);  \
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);   \
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT); \
    _CrtDumpMemoryLeaks();

int main() {
    std::cout << "Hello World!\n";
    int *x = (int *)malloc(sizeof(int));
    *x     = 7;
    printf("%d\n", *x);
    x    = (int *)calloc(3, sizeof(int));
    x[0] = 0x3F;
    x[1] = 0xABCD;
    x[2] = 0xDEADBEEF;
    printf("%d %d %d\n", x[0], x[1], x[2]);
    MEMLEAK_REPORT
}
