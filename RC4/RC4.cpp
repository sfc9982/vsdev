#include <iostream>
#include <string>

#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/arc4.h>

int main() {
    using namespace CryptoPP;

    AutoSeededRandomPool prng;
    HexEncoder           encoder(new FileSink(std::cout));
    std::string          plain("ARC4 stream cipher test"), cipher, recover;

    SecByteBlock key(16);
    prng.GenerateBlock(key, key.size());

    std::cout << "Key: ";
    encoder.Put((const byte *)key.data(), key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    // Encryption object
    Weak1::ARC4::Encryption enc;
    enc.SetKey(key, key.size());

    std::getline(std::cin, plain);

    // Perform the encryption
    cipher.resize(plain.size());
    enc.ProcessData((byte *)&cipher[0], (const byte *)&plain[0], plain.size());

    std::cout << "Plain: " << plain << std::endl;

    std::cout << "Cipher: ";
    encoder.Put((const byte *)&cipher[0], cipher.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "Self inverting: " << enc.IsSelfInverting() << std::endl;
    std::cout << "Resynchronizable: " << enc.IsResynchronizable() << std::endl;

    Weak1::ARC4::Decryption dec;
    dec.SetKey(key, key.size());

    // Perform the decryption
    recover.resize(cipher.size());
    dec.ProcessData((byte *)&recover[0], (const byte *)&cipher[0], cipher.size());

    std::cout << "Recovered: " << recover << std::endl;

    return 0;
}
