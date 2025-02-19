#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "Request.h"

/**
 * @brief Represents a registration request.
 *
 * This class inherits from the base class 'Request' and provides specific
 * functionality for registration requests, including serialization.
 */
class Registration : public Request
{
public:
    /**
     * @brief Constructor for Registration.
     *
     * @param id The unique identifier for the request.
     * @param version The version of the registration request.
     * @param code The request code indicating the type of request.
     * @param payload_size The size of the payload in the request.
     * @param name The name associated with the registration request.
     */
    Registration(const std::array<char, 16>& id, char version,
        RequestCode code, unsigned int payload_size, std::string name);

    /**
     * @brief Serializes the Registration object into a vector of characters.
     *
     * @return A vector of characters representing the serialized form of the object.
     */
    std::vector<char> serialize() const;

protected:

private:
    /**
     * @brief Member variable for storing the name associated with the registration request.
     */
    std::string Name;
};

#endif // REGISTRATION_H
