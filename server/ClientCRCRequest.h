#ifndef CLIENT_CRC_REQUEST_H
#define CLIENT_CRC_REQUEST_H

#include "Request.h"

/**
 * @brief Represents a CRC (Cyclic Redundancy Check) request from a client.
 *
 * This class inherits from the base class 'Request' and provides specific
 * functionality for CRC requests, including serialization.
 */
class ClientCRCRequest : public Request {
public:
    /**
     * @brief Constructor for ClientCRCRequest.
     *
     * @param id The unique identifier for the request.
     * @param version The version of the CRC request.
     * @param code The request code indicating the type of request.
     * @param payload_size The size of the payload in the request.
     * @param file_name The name of the file associated with the request.
     */
    ClientCRCRequest(std::array<char, 16>& id,
        char version, RequestCode code, unsigned int payload_size,
        std::array<char, 255>& file_name);

    /**
     * @brief Serializes the ClientCRCRequest object into a vector of characters.
     *
     * @return A vector of characters representing the serialized form of the object.
     */
    std::vector<char> serialize();

private:
    std::array<char, 255> fileName; /**< The name of the file associated with the request (255 bytes). */
};

#endif
