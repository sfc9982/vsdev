#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <wincrypt.h>

#include <cstdio>
#include <cstring>

enum { BUF_LEN = 1024 };

int main() {
    constexpr bool enc = true;

    BYTE       buf[BUF_LEN];
    int        len;
    const BYTE key[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    const BYTE iv[16]  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};


    {
        FILE *fp;
        len = 0;
        if ((fp = fopen("in.txt", "rb")) != nullptr) {
            int c;
            for (len = 0; (c = fgetc(fp)) != EOF && len < BUF_LEN; len++)
                buf[len] = c;
        }
        fclose(fp);
    }

    HCRYPTPROV hProv;
    HCRYPTKEY  hkey;
    DWORD      mode = CRYPT_MODE_CBC;
    struct KeyBLOB {
        BLOBHEADER hdr;
        DWORD      cbKeySize;
        BYTE       rgbKeyData[16];
    };
    KeyBLOB kb;
    kb.hdr.bType    = PLAINTEXTKEYBLOB;
    kb.hdr.bVersion = CUR_BLOB_VERSION;
    kb.hdr.reserved = 0;
    kb.hdr.aiKeyAlg = CALG_AES_128; // AES_128
    kb.cbKeySize    = 16;
    CopyMemory(kb.rgbKeyData, key, 16);
    DWORD dwCount = len;

    if (!CryptAcquireContext(&hProv, nullptr, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0))
        if (!CryptAcquireContext(&hProv, nullptr, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_NEWKEYSET))
            return fprintf(stderr, "error0\n");
    if (!CryptImportKey(hProv, reinterpret_cast<BYTE *>(&kb), sizeof(kb), 0, 0, &hkey))
        return fprintf(stderr, "error1\n");
    if (!CryptSetKeyParam(hkey, KP_IV, iv, 0))
        return fprintf(stderr, "error2\n");
    if (!CryptSetKeyParam(hkey, KP_MODE, reinterpret_cast<BYTE *>(&mode), 0))
        return fprintf(stderr, "error3\n");
    if (enc && !CryptEncrypt(hkey, 0, true, 0, buf, &dwCount, BUF_LEN))
        return fprintf(stderr, "error4\n");
    if (!enc && !CryptDecrypt(hkey, 0, true, 0, buf, &dwCount))
        return fprintf(stderr, "error4\n");


    fprintf(stderr, "%s in.txt to out.txt\n", enc ? "encrypt" : "decrypt");

    {
        FILE *fp;
        if ((fp = fopen("out.txt", "wb")) == nullptr)
            fp = stdout;
        for (int i = 0; i < dwCount; i++) {
            if (i % 16 == 0)
                fprintf(stderr, "\n");
            fprintf(stderr, "%02x ", buf[i]);
            fprintf(fp, "%c", buf[i]);
        }
        fprintf(stderr, "\n");
        fclose(fp);
    }

    CryptDestroyKey(hkey);
    CryptReleaseContext(hProv, 0);

    return 0;
}