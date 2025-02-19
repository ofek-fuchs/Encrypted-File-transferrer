#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H

#include <string>
#include "RSAWrapper.h"
#include "Base64Wrapper.h"
#include "AESWrapper.h"
#include <iomanip>

/**
 * @brief Manages RSA key handling and generation.
 *
 * This class provides functionality for setting and retrieving RSA public and private keys,
 * generating RSA public and private keys, and hexifying buffers. It uses RSAWrapper, Base64Wrapper,
 * and AESWrapper for encryption and decryption operations.
 */
class KeyHandler
{
public:
    /**
     * @brief Constructor for KeyHandler.
     *
     * @param publicKey The initial public key for the KeyHandler.
     */
    KeyHandler(std::string publicKey);

    /**
     * @brief Converts and prints the hex representation of a buffer.
     *
     * @param buffer The buffer to be hexified.
     * @param length The length of the buffer.
     */
    static void hexify(const unsigned char* buffer, unsigned int length);

    /**
     * @brief Sets the RSA public key for the KeyHandler.
     *
     * @param publicKey The RSA public key to be set.
     */
    void setRSA_PublicKey(std::string publicKey);

    /**
     * @brief Sets the RSA private key for the KeyHandler.
     *
     * @param privateKey The RSA private key to be set.
     */
    void setRSA_PrivateKey(std::string privateKey);

    /**
     * @brief Retrieves the RSA private key from the KeyHandler.
     *
     * @return The RSA private key.
     */
    std::string getRSA_PrivateKey();

    /**
     * @brief Retrieves the RSA public key from the KeyHandler.
     *
     * @return The RSA public key.
     */
    std::string getRSA_PublicKey();

    /**
     * @brief Generates and retrieves a new RSA public key.
     *
     * @return The generated RSA public key.
     */
    std::string RSA_generatePublicKey();

    /**
     * @brief Generates and retrieves a new RSA private key.
     *
     * @return The generated RSA private key.
     */
    std::string RSA_generatePrivateKey();

private:
    /**
     * @brief Member variable for storing the RSA public key.
     */
    std::string RSApublicKey;

    /**
     * @brief Member variable for storing the RSA private key.
     */
    std::string RSAprivateKey;

    /**
     * @brief Member variable representing the RSA private key decryptor.
     */
    RSAPrivateWrapper rsapriv;

    /**
     * @brief Member variable representing the RSA public key encryptor.
     */
    RSAPublicWrapper rsapub;
};

#endif // KEY_HANDLER_H
