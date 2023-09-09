#include <Windows.h>

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/tea.h>

#include "TEA.h"

#define CIPHER TEA
#define CIPHER_MODE ECB

#ifdef CIPHER_MODE
#if CIPHER_MODE == CBC
#define CIPHER_NAME CBC_Mode
#define IV
#elif CIPHER_MODE == ECB
#define CIPHER_NAME ECB_Mode
#undef IV
#endif
#endif

int main(int /*argc*/, char * /*argv*/[]) {
    try {
        byte key[CryptoPP::CIPHER::DEFAULT_KEYLENGTH];
        byte iv[CryptoPP::CIPHER::BLOCKSIZE];

        ::memset(key, 0x01, CryptoPP::CIPHER::DEFAULT_KEYLENGTH);
        ::memset(iv, 0x01, CryptoPP::CIPHER::BLOCKSIZE);

        std::string PlainText = "In God We Trust.";
        std::string CipherText;
        std::string RecoveredText;

#ifdef IV
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Encryption Encryptor(key, sizeof(key), iv);
#else
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Encryption Encryptor(key, sizeof(key));
#endif // IV

        CryptoPP::StringSource(
                PlainText,
                true,
                new CryptoPP::StreamTransformationFilter(Encryptor, new CryptoPP::StringSink(CipherText)));
#ifdef IV
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Decryption Decryptor(key, sizeof(key), iv);
#else
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Decryption Decryptor(key, sizeof(key));
#endif // IV

        CryptoPP::StringSource(
                CipherText,
                true,
                new CryptoPP::StreamTransformationFilter(Decryptor, new CryptoPP::StringSink(RecoveredText)));

#ifdef _DEBUG
        std::cout << "Algorithm:" << std::endl;
        std::cout << " " << Encryptor.AlgorithmName() << std::endl;
        std::cout << "Minimum Key Size:" << std::endl;
        std::cout << " " << Encryptor.MinKeyLength() << " bytes" << std::endl;
        std::cout << std::endl;

        std::cout << "Plain Text (" << PlainText.length() << " bytes)" << std::endl;
        std::cout << " '" << PlainText << "'" << std::endl;
        std::cout << std::endl;

        std::cout << "Cipher Text Size:" << std::endl;
        std::cout << " " << CipherText.size() << " bytes" << std::endl;
        std::cout << std::endl;

        std::cout << "Cipher Text:" << std::endl;
        std::cout << " " << [&CipherText]() -> std::string {
            std::string       str;
            std::stringstream ss;

            ss << std::hex << std::uppercase;
            ss << "{";
            for (auto c : CipherText) {
                ss << "0x" << static_cast<signed>(c & 0xff);
                if (c != CipherText.back()) {
                    ss << ", ";
                } else {
                    ss << ", 0x0}";
                }
            }

            std::getline(ss, str);
            return str;
        }() << std::endl;
        std::cout << std::endl;

        std::cout << "Recovered Text:" << std::endl;
        std::cout << " '" << RecoveredText << "'" << std::endl;
        std::cout << std::endl;
#endif // _DEBUG
    } catch (CryptoPP::Exception &ex) {
        std::cerr << ex.what() << std::endl;
    }

    catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    return 0;
}