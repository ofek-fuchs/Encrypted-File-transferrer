#ifndef SEND_FILE_REQUEST_H
#define SEND_FILE_REQUEST_H

#include "Request.h"

/**
 * @brief Represents a request for sending a file.
 *
 * This class inherits from the base class 'Request' and provides specific
 * functionality for requests related to sending files, including serialization.
 */
#pragma pack(push, 1)
struct FileData {
    unsigned int contentSize; /**< The size of the file content (4 bytes). */
    std::array<char, 255> fileName; /**< The name of the file (255 bytes). */
    std::string fileContent; /**< The content of the file. */
};
#pragma pack(pop)

class SendFileRequest : public Request {
public:
    /**
     * @brief Constructor for SendFileRequest.
     *
     * @param id The unique identifier for the request.
     * @param version The version of the send file request.
     * @param code The request code indicating the type of request.
     * @param payload_size The size of the payload in the request.
     * @param content_size The size of the file content.
     * @param file_name The name of the file.
     * @param file_content The content of the file.
     */
    SendFileRequest(std::array<char, 16>& id,
        char version, RequestCode code, unsigned int payload_size, unsigned int content_size,
        std::array<char, 255>& file_name, std::string file_content);

    /**
     * @brief Serializes the SendFileRequest object into a vector of characters.
     *
     * @return A vector of characters representing the serialized form of the object.
     */
    std::vector<char> serialize();

protected:
    /**
     * @brief Member variable for storing file-related data.
     */
    FileData fileData;
};

#endif // SEND_FILE_REQUEST_H
