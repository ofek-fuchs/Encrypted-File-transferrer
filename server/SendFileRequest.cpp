#include "SendFileRequest.h"


// SendFileRequest.cpp

SendFileRequest::SendFileRequest(std::array<char, 16>& id,
    char version, RequestCode code, unsigned int payload_size,
    unsigned int content_size, std::array<char, 255>& file_name, std::string file_content) :
    Request(id, version, code, payload_size), fileData{ content_size, file_name, file_content } {

}

// Rest of your implementation...


std::vector<char> SendFileRequest::serialize() {
    // Serialize base class
    std::vector<char> baseData = Request::serialize();

    // Serialize file data
    std::vector<char> fileDataSerialized;
    fileDataSerialized.reserve(sizeof(FileData));

    // Copy contentSize
    char* contentSizePtr = reinterpret_cast<char*>(&fileData.contentSize);
    fileDataSerialized.insert(fileDataSerialized.end(), contentSizePtr, contentSizePtr + sizeof(fileData.contentSize));

    // Copy fileName
    fileDataSerialized.insert(fileDataSerialized.end(), fileData.fileName.begin(), fileData.fileName.end());

    // Copy fileContent
    fileDataSerialized.insert(fileDataSerialized.end(), fileData.fileContent.begin(), fileData.fileContent.end());

    // Concatenate base class and file data
    baseData.insert(baseData.end(), fileDataSerialized.begin(), fileDataSerialized.end());

    return baseData;
}
