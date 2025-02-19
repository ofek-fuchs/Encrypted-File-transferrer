#include "RequestParser.h"




std::unique_ptr<Request> RequestParser::createRequest(const RequestCode code, std::string name) {
   /*unsigned int client ID, Version,code,payload size, payload*/
    const char version = 1;
    std::array<char, 16> id = {0}; 
    unsigned int register_payload_size = 255;

    switch (code) {
    case RequestCode::Register:
        return createRegRequest(id, version, code, register_payload_size, name);
        break;
 
    default:
        return NULL;
    }

}
std::unique_ptr<Request> RequestParser::createRegRequest(const std::array<char, 16>& id, char version, RequestCode code, unsigned int payload_size, std::string name)
{
     std::unique_ptr<Request> regRequest = std::make_unique <Registration>(id,version,code,payload_size,name);
	 return regRequest;
}

ServerRequest RequestParser::unpackBasicServerRequest(std::vector<char> requestBinary) {
    const int SERVER_REQUEST_SIZE = 7;
    size_t newSize = std::min(requestBinary.size(), static_cast<size_t>(SERVER_REQUEST_SIZE));
    auto basicRequestBinary = requestBinary;
    basicRequestBinary.resize(newSize);
    ServerRequest basicData = ServerRequest::deserialize(basicRequestBinary);
   
    return basicData;

}
