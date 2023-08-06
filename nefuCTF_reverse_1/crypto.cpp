﻿#include "stdafx.h"

void hexdumpStr(std::string str) {
    std::cout << std::hex << std::uppercase;
    for (auto c : str) {
        std::cout << "0x" << (static_cast<unsigned>(c) & 0xff) << ", ";
    }
    std::cout << std::endl;
}

std::string encryptStr(std::string str) {
    std::string ret;
    try {
        byte key[CryptoPP::CIPHER::DEFAULT_KEYLENGTH];
        byte iv[CryptoPP::CIPHER::BLOCKSIZE];

        ::memset(key, 0x01, CryptoPP::CIPHER::DEFAULT_KEYLENGTH);
        ::memset(iv, 0x01, CryptoPP::CIPHER::BLOCKSIZE);

        const std::string &PlainText = str;
        std::string        CipherText;

#ifdef IV
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Encryption Encryptor(key, sizeof(key), iv);
#else
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Encryption Encryptor(key, sizeof(key));
#endif // IV

        CryptoPP::StringSource(
                PlainText,
                true,
                new CryptoPP::StreamTransformationFilter(Encryptor, new CryptoPP::StringSink(CipherText)));

        ret = CipherText;

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
            for (auto c : CipherText) {
                ss << "0x" << static_cast<signed>(c) << ", ";
            }

            std::getline(ss, str);
            return str;
        }()
                << std::endl;
        std::cout << std::endl;
#endif // _DEBUG
    } catch (CryptoPP::Exception &ex) {
        std::cerr << ex.what() << std::endl;
    }

    catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    return ret;
}

std::string decryptStr(std::string str) {
    std::string ret;

    try {
        byte key[CryptoPP::CIPHER::DEFAULT_KEYLENGTH];
        byte iv[CryptoPP::CIPHER::BLOCKSIZE];

        ::memset(key, 0x01, CryptoPP::CIPHER::DEFAULT_KEYLENGTH);
        ::memset(iv, 0x01, CryptoPP::CIPHER::BLOCKSIZE);

        const std::string &CipherText = str;
        std::string        RecoveredText;

#ifdef IV
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Decryption Decryptor(key, sizeof(key), iv);
#else
        CryptoPP::CIPHER_NAME<CryptoPP::CIPHER>::Decryption Decryptor(key, sizeof(key));
#endif // IV

        CryptoPP::StringSource(
                CipherText,
                true,
                new CryptoPP::StreamTransformationFilter(Decryptor, new CryptoPP::StringSink(RecoveredText)));

        ret = RecoveredText;

#ifdef _DEBUG
        std::cout << "Algorithm:" << std::endl;
        std::cout << " " << Decryptor.AlgorithmName() << std::endl;
        std::cout << "Minimum Key Size:" << std::endl;
        std::cout << " " << Decryptor.MinKeyLength() << " bytes" << std::endl;
        std::cout << std::endl;

        std::cout << "Cipher Text Size:" << std::endl;
        std::cout << " " << CipherText.size() << " bytes" << std::endl;
        std::cout << std::endl;

        std::cout << "Cipher Text:" << std::endl;
        hexdumpStr(CipherText);
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

    return ret;
}