#pragma once

// Include necessary Crypto++ headers
#include <osrng.h>
#include <rsa.h>

// Include standard library headers
#include <string>

// Definition of RSAPublicWrapper class
class RSAPublicWrapper
{
public:
    // Constants for key size and bits
    static const unsigned int KEYSIZE = 160;
    static const unsigned int BITS = 1024;

private:
    // Random number generator and public key storage
    CryptoPP::AutoSeededRandomPool _rng;
    CryptoPP::RSA::PublicKey _publicKey;

    // Private copy constructor and copy assignment operator to prevent copying
    RSAPublicWrapper(const RSAPublicWrapper& rsapublic);
    RSAPublicWrapper& operator=(const RSAPublicWrapper& rsapublic);

public:
    // Constructors and destructor
    RSAPublicWrapper(const char* key, unsigned int length);
    RSAPublicWrapper(const std::string& key);
    ~RSAPublicWrapper();

    // Get the public key as a string
    std::string getPublicKey() const;

    // Get the public key and store it in a buffer
    char* getPublicKey(char* keyout, unsigned int length) const;

    // Encrypt plaintext using the public key
    std::string encrypt(const std::string& plain);

    // Encrypt plaintext (in char array form) using the public key
    std::string encrypt(const char* plain, unsigned int length);
};

// Definition of RSAPrivateWrapper class
class RSAPrivateWrapper
{
public:
    // Constant for key bits
    static const unsigned int BITS = 1024;

private:
    // Random number generator and private key storage
    CryptoPP::AutoSeededRandomPool _rng;
    CryptoPP::RSA::PrivateKey _privateKey;

    // Private copy constructor and copy assignment operator to prevent copying
    RSAPrivateWrapper(const RSAPrivateWrapper& rsaprivate);
    RSAPrivateWrapper& operator=(const RSAPrivateWrapper& rsaprivate);

public:
    // Constructors and destructor
    RSAPrivateWrapper();
    RSAPrivateWrapper(const char* key, unsigned int length);
    RSAPrivateWrapper(const std::string& key);
    ~RSAPrivateWrapper();

    // Get the private key as a string
    std::string getPrivateKey() const;

    // Get the private key and store it in a buffer
    char* getPrivateKey(char* keyout, unsigned int length) const;

    // Get the public key from the private key and store it as a string
    std::string getPublicKey() const;

    // Get the public key from the private key and store it in a buffer
    char* getPublicKey(char* keyout, unsigned int length) const;

    // Decrypt ciphertext using the private key
    std::string decrypt(const std::string& cipher);

    // Decrypt ciphertext (in char array form) using the private key
    std::string decrypt(const char* cipher, unsigned int length);
};
