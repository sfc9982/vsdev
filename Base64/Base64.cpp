#include <iostream>
#include <string>

#include "turbob64.h"

#pragma comment(lib, "TurboBase64")

enum {
    BUFLEN = 256
};

int main() {
    std::string    in              = "base64 test string.";
    char           buf_in[BUFLEN]  = {};
    char           buf_out[BUFLEN] = {};
    unsigned char *in_ptr          = nullptr;
    unsigned char *out_ptr         = nullptr;

    tb64ini(0, 0);

    size_t len = in.size();
    if (!strcpy_s(buf_in, in.c_str())) {
        in_ptr = reinterpret_cast<unsigned char *>(buf_in);
    } else {
        return EXIT_FAILURE;
    }
    out_ptr = reinterpret_cast<unsigned char *>(buf_out);

    tb64enc(in_ptr, len, out_ptr);
    puts(buf_out);

    return EXIT_SUCCESS;
}
