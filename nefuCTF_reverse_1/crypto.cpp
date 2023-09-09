#include "stdafx.h"

#include "crypto.h"

bool        useCustomKey = false;
std::string CustomKey;

void hexdumpStr(std::string str) {
    std::cout << "Hex Dump:" << std::endl;
    std::cout << std::hex << std::uppercase;
    std::cout << "{";
    for (auto c : str) {
        std::cout << "0x" << (static_cast<unsigned>(c) & 0xff)
                  << [&str, c]() -> std::string {
            if (c != str.back())
                return std::string{", "};
            return std::string{", 0x0}"};
        }();
    }
    std::cout << std::endl;
}

std::string encryptStr(const std::string &str) {
    std::string ret;
    try {
        byte key[CryptoPP::CIPHER::DEFAULT_KEYLENGTH];
        byte iv[CryptoPP::CIPHER::BLOCKSIZE];

        memset(key, 0x01, CryptoPP::CIPHER::DEFAULT_KEYLENGTH);
        memset(iv, 0x01, CryptoPP::CIPHER::BLOCKSIZE);

        if (useCustomKey) {
            memcpy(key, CustomKey.c_str(), sizeof(key));
        }
        const std::string &PlainText = str;
        std::string        CipherText;

#ifdef IV
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Encryption Encryptor(key, sizeof(key), iv);
#else
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Encryption Encryptor(key, sizeof(key));
#endif // IV

        CryptoPP::StringSource(PlainText, true, new CryptoPP::StreamTransformationFilter(Encryptor, new CryptoPP::StringSink(CipherText)));

        ret = CipherText;
    } catch (CryptoPP::Exception &ex) {
        std::cerr << ex.what() << std::endl;
    }

    catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    return ret;
}

std::string decryptStr(const std::string &str) {
    std::string ret;

    try {
        byte key[CryptoPP::CIPHER::DEFAULT_KEYLENGTH];
        byte iv[CryptoPP::CIPHER::BLOCKSIZE];

        memset(key, 0x01, CryptoPP::CIPHER::DEFAULT_KEYLENGTH);
        memset(iv, 0x01, CryptoPP::CIPHER::BLOCKSIZE);

        if (useCustomKey) {
            memcpy(key, CustomKey.c_str(), sizeof(key));
        }
      
      
        const std::string &CipherText = str;
        std::string        RecoveredText;

#ifdef IV
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Decryption Decryptor(key, sizeof(key), iv);
#else
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Decryption Decryptor(key, sizeof(key));
#endif // IV

        CryptoPP::StringSource(CipherText, true, new CryptoPP::StreamTransformationFilter(Decryptor, new CryptoPP::StringSink(RecoveredText)));

        ret = RecoveredText;
    } catch (CryptoPP::Exception &ex) {
        std::cerr << ex.what() << std::endl;
    }

    catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    return ret;
}
