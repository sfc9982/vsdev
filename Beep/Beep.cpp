#include <Windows.h>
#include <iostream>
#include <utilapiset.h>

int main() {
    while (true) {
        Beep(423, 1000);
        Beep(850, 1000);
    }

    Sleep(1000);
}
