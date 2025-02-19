#ifndef SERVER_REQUEST_H
#define SERVER_REQUEST_H

#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include "RequestCode.h"

/**
 * @brief Represents a generic request from the server.
 *
 * This class provides a common structure for various types of server requests,
 * including version, request code, and payload size. It includes functionality
 * for serializing and deserializing server requests.
 */
#pragma pack(push, 1)
struct SData {
    char version; /**< The version of the server request (1 byte). */
    RequestCode code; /**< The request code indicating the type of request (2 bytes). */
    unsigned int payload_size; /**< The size of the payload in the server request (4 bytes). */
};
#pragma pack(pop)

class ServerRequest
{
public:
    /**
     * @brief Constructor for ServerRequest.
     *
     * @param version The version of the server request.
     * @param code The request code indicating the type of request.
     * @param payload_size The size of the payload in the server request.
     */
    ServerRequest(char version, RequestCode code, unsigned int payload_size);

    /**
     * @brief Serializes the ServerRequest object into a vector of characters.
     *
     * @return A vector of characters representing the serialized form of the object.
     */
    virtual std::vector<char> serialize() const;

    /**
     * @brief Deserializes a vector of characters into a ServerRequest object.
     *
     * @param binaryData The binary representation of the server request.
     * @return A ServerRequest object representing the deserialized request.
     */
    static ServerRequest deserialize(const std::vector<char>& binaryData);

    /**
     * @brief Gets the payload size of the server request.
     *
     * @return The payload size.
     */
    const unsigned int getPayloadSize();

    /**
     * @brief Gets the request code of the server request.
     *
     * @return The request code.
     */
    const RequestCode getCode();

private:
    /**
     * @brief Member variable for storing the data of the server request.
     */
    SData data;
};

#endif // SERVER_REQUEST_H
