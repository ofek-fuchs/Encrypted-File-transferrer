// Request.cpp
#include "Request.h"

Request::Request(const std::array<char, 16>& id, char version, RequestCode code, unsigned int payload_size)
    : data{ id, version, code, payload_size } {}

std::vector<char> Request::serialize() const
{
    // Convert your request data to binary format
    Data requestData = { data.client_ID, data.version, data.code,(data.payload_size)};

    // Convert the data structure to binary data with explicit packing
    std::vector<char> binaryData(sizeof(Data));
    char* binaryPtr = reinterpret_cast<char*>(&requestData);

    for (size_t i = 0; i < sizeof(Data); ++i)
    {
        binaryData[i] = *binaryPtr++;

    }


    
    return binaryData;
}

