#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <string>
#include <cstdlib>
#include "Registration.h"
#include "PublicKeyRequest.h"
#include "ServerRequest.h"
#include "RequestCode.h"
#include "SendFileRequest.h"
#include "KeyHandler.h"

/**
 * @brief Parses and creates various types of requests.
 *
 * This class provides static methods for creating different types of requests
 * based on the request code. It also includes a method for unpacking basic server requests.
 */
class RequestParser {
public:
    /**
     * @brief Creates a request based on the provided request code and name.
     *
     * @param code The request code indicating the type of request.
     * @param name The name associated with the request (if applicable).
     * @return A unique pointer to the created Request object.
     */
    static std::unique_ptr<Request> createRequest(const RequestCode code, std::string name);

    /**
     * @brief Unpacks a basic server request from its binary representation.
     *
     * @param requestBinary The binary representation of the server request.
     * @return A ServerRequest object representing the unpacked request.
     */
    static ServerRequest unpackBasicServerRequest(std::vector<char> requestBinary);

protected:

private:
    /**
     * @brief Creates a registration request based on the provided parameters.
     *
     * @param id The unique identifier for the request.
     * @param version The version of the request.
     * @param code The request code indicating the type of request.
     * @param payload_size The size of the payload in the request.
     * @param name The name associated with the registration request.
     * @return A unique pointer to the created Registration object.
     */
    static std::unique_ptr<Request> createRegRequest(const std::array<char, 16>& id, char version, RequestCode code, unsigned int payload_size, std::string name);
};

#endif // REQUEST_PARSER_H
