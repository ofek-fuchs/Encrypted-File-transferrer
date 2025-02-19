// Request.cpp
#include "ServerRequest.h"


ServerRequest::ServerRequest(char version, RequestCode code, unsigned int payload_size)
    : data{ version, code, payload_size } {}


std::vector<char> ServerRequest::serialize() const
{
    // Convert your request data to binary format
    SData requestData = {data.version, data.code,(data.payload_size) };

    // Convert the data structure to binary data with explicit packing
    std::vector<char> binaryData(sizeof(SData));
    char* binaryPtr = reinterpret_cast<char*>(&requestData);

    for (size_t i = 0; i < sizeof(SData); ++i)
    {
        binaryData[i] = *binaryPtr++;
    }

    // Convert RequestCode to little-endian before serialization
    std::uint16_t littleEndianCode = static_cast<std::uint16_t>(requestData.code);
    binaryData[sizeof(std::array<char, 16>) + sizeof(char) + 0] = littleEndianCode & 0xFF;
    binaryData[sizeof(std::array<char, 16>) + sizeof(char) + 1] = (littleEndianCode >> 8) & 0xFF;


    return binaryData;
}

ServerRequest ServerRequest::deserialize(const std::vector<char>& binaryData) {
    if (binaryData.size() != sizeof(SData)) {
        // Handle error: The binary data size doesn't match the expected size
        // You might want to throw an exception or handle it as appropriate
        throw std::runtime_error("Invalid binary data size");
    }

    // Copy binary data to a temporary buffer
    char buffer[sizeof(SData)];
    for (size_t i = 0; i < sizeof(SData); ++i) {
        buffer[i] = binaryData[i];
    }

    // Convert the buffer back to the data structure
    SData* requestData = reinterpret_cast<SData*>(buffer);

    // Return a new ServerRequest instance with the deserialized data
    return ServerRequest(requestData->version, requestData->code, requestData->payload_size);
}

const unsigned int ServerRequest::getPayloadSize()
{
    return data.payload_size;
}

const RequestCode ServerRequest::getCode()
{
    return data.code;
}

