#include "KeyHandler.h"

/**
 * @brief Converts a byte buffer to its hexadecimal representation.
 *
 * This function takes a byte buffer and its length, and prints the hexadecimal
 * representation of the buffer to the standard output. It prints 16 bytes per line,
 * with each byte padded with zeros and separated by spaces.
 *
 * @param buffer A pointer to the byte buffer.
 * @param length The length of the byte buffer.
 */
void KeyHandler::hexify(const unsigned char* buffer, unsigned int length)
{
    std::ios::fmtflags f(std::cout.flags());
    std::cout << std::hex;
    for (size_t i = 0; i < length; i++)
        std::cout << std::setfill('0') << std::setw(2) << (0xFF & buffer[i]) << (((i + 1) % 16 == 0) ? "\n" : " ");
    std::cout << std::endl;
    std::cout.flags(f);
}

void KeyHandler::setRSA_PrivateKey(std::string privateKey)
{
    this->RSAprivateKey = privateKey;
}
std::string KeyHandler::getRSA_PrivateKey() {
    return this->RSAprivateKey;
}

/**
 * @brief Sets the RSA public key for the KeyHandler instance.
 *
 * This function sets the RSA public key for the KeyHandler instance.
 *
 * @param publicKey The RSA public key to be set.
 */
void KeyHandler::setRSA_PublicKey(std::string publicKey)
{
    this->RSApublicKey = publicKey;
}

/**
 * @brief Gets the RSA public key from the KeyHandler instance.
 *
 * This function returns the RSA public key stored in the KeyHandler instance.
 *
 * @return The RSA public key.
 */
std::string KeyHandler::getRSA_PublicKey()
{
    return RSApublicKey;
}

KeyHandler::KeyHandler(std::string publicKey) : rsapriv(),rsapub(publicKey)
{
    this->RSApublicKey = publicKey;
}


/**
 * @brief Generates the RSA public key.
 *
 * This function generates the RSA public key using the underlying rsapriv
 * and returns it as a string.
 *
 * @return The generated RSA public key.
 */
std::string KeyHandler::RSA_generatePublicKey()
{
    std::string tempKey = rsapriv.getPublicKey();
    return tempKey;
}

/**
 * @brief Generates the RSA private key in Base64 encoding.
 *
 * This function generates the RSA private key using the underlying rsapriv,
 * encodes it in Base64, and returns the Base64-encoded private key as a string.
 *
 * @return The Base64-encoded RSA private key.
 */
std::string KeyHandler::RSA_generatePrivateKey()
{
    std::string key = rsapriv.getPrivateKey();
    std::string base64key = Base64Wrapper::encode(key);
    return base64key;
}
