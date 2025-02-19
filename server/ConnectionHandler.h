#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <vector>
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include "FileReader.h"
#include "RequestParser.h"
#include "RequestCode.h"

/**
 * @brief Represents a connection handler for managing communication with a server.
 *
 * This class provides functionality for initializing, connecting, and closing a connection
 * with a server. It also includes methods for receiving and sending requests.
 */
class ConnectionHandler {
public:
    /**
     * @brief Constructor for ConnectionHandler.
     *
     * @param serverAddress The address of the server to connect to.
     * @param port The port number to use for the connection.
     */
    ConnectionHandler(const char* serverAddress, const unsigned short port);

    /**
     * @brief Destructor for ConnectionHandler.
     */
    ~ConnectionHandler();

    /**
     * @brief Initializes the connection handler and required resources.
     *
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Connects to the server.
     *
     * @return True if the connection is successful, false otherwise.
     */
    bool connectToServer();

    /**
     * @brief Closes the connection to the server.
     */
    void closeConnection();

    /**
     * @brief Receives a request from the server.
     *
     * @return A vector of characters representing the received request.
     */
    std::vector<char> receiveRequest();

    /**
     * @brief Sends a request to the server.
     *
     * @param reqBinary The binary representation of the request to be sent.
     * @return True if the request is sent successfully, false otherwise.
     */
    bool sendRequest(std::vector<char> reqBinary);

private:
    /**
     * @brief Helper method to print the contents of a vector of characters.
     *
     * @param vec The vector of characters to be printed.
     */
    void printVector(const std::vector<char>& vec);

    std::string serverAddress; /**< The address of the server to connect to. */
    const unsigned short serverPort; /**< The port number to use for the connection. */
    SOCKET clientSocket; /**< The socket for communication with the server. */
};

#endif // CONNECTION_HANDLER_H
