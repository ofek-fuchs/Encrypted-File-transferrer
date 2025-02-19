// Include the header file that declares the classes and necessary dependencies
#include "RSAWrapper.h"

// Implementation of RSAPublicWrapper class

// Constructor for RSAPublicWrapper taking a key and its length
RSAPublicWrapper::RSAPublicWrapper(const char* key, unsigned int length)
{
    // Load the public key from the given key string
    CryptoPP::StringSource ss(reinterpret_cast<const CryptoPP::byte*>(key), length, true);
    _publicKey.Load(ss);
}

// Constructor for RSAPublicWrapper taking a key as a string
RSAPublicWrapper::RSAPublicWrapper(const std::string& key)
{
    // Load the public key from the given key string
    CryptoPP::StringSource ss(key, true);
    _publicKey.Load(ss);
}

// Destructor for RSAPublicWrapper
RSAPublicWrapper::~RSAPublicWrapper()
{
}

// Get the public key as a string
std::string RSAPublicWrapper::getPublicKey() const
{
    std::string key;
    CryptoPP::StringSink ss(key);
    _publicKey.Save(ss);
    return key;
}

// Get the public key and store it in a buffer
char* RSAPublicWrapper::getPublicKey(char* keyout, unsigned int length) const
{
    CryptoPP::ArraySink as(reinterpret_cast<CryptoPP::byte*>(keyout), length);
    _publicKey.Save(as);
    return keyout;
}

// Encrypt the given plaintext using the public key
std::string RSAPublicWrapper::encrypt(const std::string& plain)
{
    std::string cipher;
    CryptoPP::RSAES_OAEP_SHA_Encryptor e(_publicKey);
    CryptoPP::StringSource ss(plain, true, new CryptoPP::PK_EncryptorFilter(_rng, e, new CryptoPP::StringSink(cipher)));
    return cipher;
}

// Encrypt the given plaintext (in char array form) using the public key
std::string RSAPublicWrapper::encrypt(const char* plain, unsigned int length)
{
    std::string cipher;
    CryptoPP::RSAES_OAEP_SHA_Encryptor e(_publicKey);
    CryptoPP::StringSource ss(reinterpret_cast<const CryptoPP::byte*>(plain), length, true,
        new CryptoPP::PK_EncryptorFilter(_rng, e, new CryptoPP::StringSink(cipher)));
    return cipher;
}

// Implementation of RSAPrivateWrapper class

// Default constructor for RSAPrivateWrapper, initializes a private key with BITS size
RSAPrivateWrapper::RSAPrivateWrapper()
{
    _privateKey.Initialize(_rng, BITS);
}

// Constructor for RSAPrivateWrapper taking a key and its length
RSAPrivateWrapper::RSAPrivateWrapper(const char* key, unsigned int length)
{
    // Load the private key from the given key string
    CryptoPP::StringSource ss(reinterpret_cast<const CryptoPP::byte*>(key), length, true);
    _privateKey.Load(ss);
}

// Constructor for RSAPrivateWrapper taking a key as a string
RSAPrivateWrapper::RSAPrivateWrapper(const std::string& key)
{
    // Load the private key from the given key string
    CryptoPP::StringSource ss(key, true);
    _privateKey.Load(ss);
}

// Destructor for RSAPrivateWrapper
RSAPrivateWrapper::~RSAPrivateWrapper()
{
}

// Get the private key as a string
std::string RSAPrivateWrapper::getPrivateKey() const
{
    std::string key;
    CryptoPP::StringSink ss(key);
    _privateKey.Save(ss);
    return key;
}

// Get the private key and store it in a buffer
char* RSAPrivateWrapper::getPrivateKey(char* keyout, unsigned int length) const
{
    CryptoPP::ArraySink as(reinterpret_cast<CryptoPP::byte*>(keyout), length);
    _privateKey.Save(as);
    return keyout;
}

// Get the public key from the private key and store it as a string
std::string RSAPrivateWrapper::getPublicKey() const
{
    CryptoPP::RSAFunction publicKey(_privateKey);
    std::string key;
    CryptoPP::StringSink ss(key);
    publicKey.Save(ss);
    return key;
}

// Get the public key from the private key and store it in a buffer
char* RSAPrivateWrapper::getPublicKey(char* keyout, unsigned int length) const
{
    CryptoPP::RSAFunction publicKey(_privateKey);
    CryptoPP::ArraySink as(reinterpret_cast<CryptoPP::byte*>(keyout), length);
    publicKey.Save(as);
    return keyout;
}

// Decrypt the given ciphertext using the private key
std::string RSAPrivateWrapper::decrypt(const std::string& cipher)
{
    std::string decrypted;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(_privateKey);
    CryptoPP::StringSource ss_cipher(cipher, true, new CryptoPP::PK_DecryptorFilter(_rng, d, new CryptoPP::StringSink(decrypted)));
    return decrypted;
}

// Decrypt the given ciphertext (in char array form) using the private key
std::string RSAPrivateWrapper::decrypt(const char* cipher, unsigned int length)
{
    std::string decrypted;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(_privateKey);
    CryptoPP::StringSource ss_cipher(reinterpret_cast<const CryptoPP::byte*>(cipher), length, true,
        new CryptoPP::PK_DecryptorFilter(_rng, d, new CryptoPP::StringSink(decrypted)));
    return decrypted;
}
