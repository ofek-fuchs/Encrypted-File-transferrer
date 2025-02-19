#ifndef UTIL_H
#define UTIL_H

#define NOMINMAX
#define SERVER_REQUEST_SIZE 7
#define PAYLOAD_SIZE_REQUEST_1026 415
#define PAYLOAD_SIZE_REQUEST_1028 259
#define PAYLOAD_SIZE_CRC 255
#define PAYLOAD_SIZE_RECONNECT 255
#define BASE_DATA_SIZE 23
#define MAX_REQUEST_SIZE 4096
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <winsock2.h> /*Include the Windows Socket API*/ 
#include <ws2tcpip.h>
#include <fstream>
#include <algorithm>
#include "Util.h"
#include "FileReader.h"
#include "RequestParser.h"
#include "RequestCode.h"
#include "Base64Wrapper.h"
#include "RSAWrapper.h"
#include "AESWrapper.h"
#include "KeyHandler.h"
#include "ConnectionHandler.h"
#include "ClientCRCRequest.h"
#include "cksum_new.h"

/**
 * @brief Prints the hex representation of a buffer.
 *
 * @param buffer The buffer to be hexified.
 * @param length The length of the buffer.
 */
void hexify(const unsigned char* buffer, unsigned int length)
{
	std::ios::fmtflags f(std::cout.flags());
	std::cout << std::hex;
	for (size_t i = 0; i < length; i++)
		std::cout << std::setfill('0') << std::setw(2) << (0xFF & buffer[i]) << (((i + 1) % 16 == 0) ? "\n" : " ");
	std::cout << std::endl;
	std::cout.flags(f);
}
/**
 * @brief Converts a hex representation string to an array of characters.
 *
 * @param hexRepresentation The hex representation string to convert.
 * @return An array of characters representing the converted hex string.
 */
std::array<char, 16> hexToAscii(const std::string& hexRepresentation) {
    std::array<char, 16> result = {}; // Initialize with zeros

    // Process two characters at a time (one byte)
    for (std::size_t i = 0; i < hexRepresentation.length() && i < result.size() * 2; i += 2) {
        // Extract two characters from the hex string
        std::string byteString = hexRepresentation.substr(i, 2);

        // Convert the two characters to an integer
        int byteValue;
        std::istringstream(byteString) >> std::hex >> byteValue;

        // Convert the integer to a char and set in the result array
        result[i / 2] = static_cast<char>(byteValue);
    }

    return result;
}
#endif