// Registration.cpp
#include "Registration.h"

Registration::Registration(const std::array<char, 16>& id, char version, RequestCode code, unsigned int payload_size, std::string name)
    : Request(id, version, code, payload_size), Name(name)
{
 
}

std::vector<char> Registration::serialize() const
{
    // Serialize the base class data using the base class method
    std::vector<char> baseData = Request::serialize();

    // Additional serialization for the Registration class
    std::vector<char> nameData(Name.begin(), Name.end());

    // Combine the base class data and additional data
    baseData.insert(baseData.end(), nameData.begin(), nameData.end());

    return baseData;
}
