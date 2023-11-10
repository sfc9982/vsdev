#include <cstdint>
#include <cstdio>

int main(int argc, char *argv[]) {
    uint8_t start = 0x02;
    uint8_t a     = start;
    for (int i = 1;; i++) {
        int newbit = (((a >> 6) ^ (a >> 5)) & 1);
        a          = ((a << 1) | newbit) & 0x7f;
        printf("0x%X\n", a);
        if (a == start) {
            printf("repetition period is %d\n", i);
            break;
        }
    }
}