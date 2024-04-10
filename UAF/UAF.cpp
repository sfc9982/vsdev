#include <stdio.h>
#include <stdlib.h>

void hello(void) {
    puts("#1 Original Function");
}

void hack(void) {
    puts("#2 Hacked Function");
}

typedef void (*fun_ptr)(void);
fun_ptr *ptr;

int main() {
    ptr  = (fun_ptr *)malloc(128);
    *ptr = hello;
    (*ptr)();
    free(ptr);
    *(fun_ptr *)malloc(128) = hack;
    (*ptr)();
    return 0;
}
