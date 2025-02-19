#include "ClientCRCRequest.h"


// SendFileRequest.cpp

ClientCRCRequest::ClientCRCRequest(std::array<char, 16>& id,
    char version, RequestCode code, unsigned int payload_size, std::array<char, 255>& file_name) :
    Request(id, version, code, payload_size), fileName(file_name) {

}


std::vector<char> ClientCRCRequest::serialize() {
    // Serialize base class
    std::vector<char> baseData = Request::serialize();

    // Serialize file data
    std::vector<char> fileData(fileName.begin(), fileName.end());

    // Concatenate base class and file data
    baseData.insert(baseData.end(), fileData.begin(), fileData.end());

    return baseData;
}
