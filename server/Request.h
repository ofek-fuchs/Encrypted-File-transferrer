#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include "RequestCode.h"
#include "RSAWrapper.h"
#include "KeyHandler.h"

/**
 * @brief Represents a generic request.
 *
 * This class provides a common structure for various types of requests,
 * including client ID, version, request code, and payload size. It also
 * includes functionality for serializing the request.
 */
#pragma pack(push, 1)
struct Data {
    std::array<char, 16> client_ID; /**< The unique identifier for the request (16 bytes). */
    char version; /**< The version of the request (1 byte). */
    RequestCode code; /**< The request code indicating the type of request (2 bytes). */
    unsigned int payload_size; /**< The size of the payload in the request (4 bytes). */
};
#pragma pack(pop)

class Request
{
public:
    /**
     * @brief Constructor for Request.
     *
     * @param id The unique identifier for the request.
     * @param version The version of the request.
     * @param code The request code indicating the type of request.
     * @param payload_size The size of the payload in the request.
     */
    Request(const std::array<char, 16>& id, char version, RequestCode code, unsigned int payload_size);

    /**
     * @brief Serializes the Request object into a vector of characters.
     *
     * @return A vector of characters representing the serialized form of the object.
     */
    virtual std::vector<char> serialize() const;

protected:
    /**
     * @brief Member variable for storing the data of the request.
     */
    Data data;
};

#endif // REQUEST_H
