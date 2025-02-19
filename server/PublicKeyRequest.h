#ifndef PUBLIC_KEY_REQUEST_H
#define PUBLIC_KEY_REQUEST_H

#include "Request.h"

/**
 * @brief Represents a request for a public key.
 *
 * This class inherits from the base class 'Request' and provides specific
 * functionality for requests related to public keys, including serialization.
 */
class PublicKeyRequest : public Request {

public:
    /**
     * @brief Constructor for PublicKeyRequest.
     *
     * @param id The unique identifier for the request.
     * @param version The version of the public key request.
     * @param code The request code indicating the type of request.
     * @param payload_size The size of the payload in the request.
     * @param name The name associated with the public key request.
     * @param key The public key associated with the request.
     */
    PublicKeyRequest(std::array<char, 16>& id,
        char version, RequestCode code, unsigned int payload_size,
        std::string name, std::string key);

    /**
     * @brief Serializes the PublicKeyRequest object into a vector of characters.
     *
     * @return A vector of characters representing the serialized form of the object.
     */
    std::vector<char> serialize();

private:
    /**
     * @brief Member variable for storing the public key.
     */
    std::string pubkey;

    /**
     * @brief Member variable for storing the user name associated with the request.
     */
    std::string user_name;
};

#endif // PUBLIC_KEY_REQUEST_H
