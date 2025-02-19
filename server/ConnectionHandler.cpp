#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler(const char* serverAddress, const unsigned short port) : clientSocket(INVALID_SOCKET), serverAddress(serverAddress), serverPort(port) {}

ConnectionHandler::~ConnectionHandler() {
    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
    }
    WSACleanup();
}

bool ConnectionHandler::initialize() {

    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

bool ConnectionHandler::connectToServer() {

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverAddress.c_str(), &(serverAddr.sin_addr));

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to the server" << std::endl;
        closesocket(clientSocket);
        return false;
    }

    return true;
}

void ConnectionHandler::closeConnection() {
    closesocket(clientSocket);
    WSACleanup(); /*Clean up Winsock*/
}

void ConnectionHandler::printVector(const std::vector<char>& vec) {
    std::cout << "Vector size: " << vec.size() << std::endl;
    for (char c : vec) {
        std::cout << static_cast<int>(c) << " ";
    }
    std::cout << std::endl;
}

bool ConnectionHandler::sendRequest(std::vector<char> reqBinary) {
    /* Send a binary request(e.g., bytes) to the server */
    const char* request_message = reqBinary.data();
    std::cerr << "Client binary message size: " << reqBinary.size() << std::endl;

        if (send(clientSocket, request_message, reqBinary.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Error sending data" << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return 0;
        }
        return 1;
}

std::vector<char> ConnectionHandler::receiveRequest() {
    char buffer[4096];
    int bytes_received;

    // Set a timeout of 1 second (adjust as needed)
    struct timeval timeout;
    timeout.tv_sec = 0;  // seconds

    if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
        std::cerr << "Error setting socket timeout" << std::endl;
        // Handle the error as needed
    }

    // Receive the response from the server
    bytes_received = recv(clientSocket, buffer, sizeof(buffer), 0);
    std::vector<char> receivedData;

    if (bytes_received == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAETIMEDOUT) {
            std::cerr << "Timeout: no response from server, connection timed out..." << std::endl;
            // Handle the timeout as needed
        }
        else {
            std::cerr << "Error receiving data" << std::endl;
            // Handle other errors as needed
        }
    }
    else {
        // Copy received data to the vector
        receivedData.assign(buffer, buffer + bytes_received);
    }

    return receivedData;
}


