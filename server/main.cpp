#include "Util.h"
#pragma comment(lib, "ws2_32.lib")

struct UserID {
	char data[16];
};

int main(char* args[]) {
	/*initializing client variables*/
	std::string addrFinder;
	std::string portFinder;
	std::string filePath;
	std::string hexRepresentation;
	std::string publicKey;
	std::string privateKey;
	std::string null_terminator = "/";
	std::string client_name = "";
	std::string aes_key;
	const std::string meFileLocation = "..\\Debug\\me.info";
	const std::string transferFileLocation = "..\\Debug\\transfer.info";
	unsigned long checkSum = 0;
	unsigned int version = 1;
	unsigned int crc_sent = 0;
	bool file_sent = false;
	unsigned short default_port = 1357;
	unsigned short server_port = default_port;
	const char* default_server_address = "127.0.0.1";
	const char* server_address = default_server_address;
	std::array<char, 16> user_id;
	std::array<char, 255> fileNameArray;
	KeyHandler* keyHandler = nullptr;
	UserID* user_ID = nullptr;

	/*---------------------------------------------------fetching server details---------------------------------------------------*/

	/* trying to read 'transfer' file content */
	if (FileReader::fileExists(transferFileLocation) && FileReader::readTransferFile(transferFileLocation, addrFinder, client_name, portFinder, filePath)) {
		std::cout << "-------------server details recieved:-------------" << std::endl;
		server_address = addrFinder.c_str();
		client_name = client_name.c_str();
		std::cout << "Server address: " << server_address << std::endl;
		std::cout << "Name: " << client_name << std::endl;
		try {
			server_port = static_cast<unsigned short>(std::stoi(portFinder));
		}
		catch (...) {
			std::cerr << "Invalid port number " << std::endl;
			return 1;
		}
		std::cout << "Port: " << server_port << std::endl;
		std::cout << "File path: " << filePath << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;
	}
	else {
		std::cout << "WARNING: failed to read 'transfer' file, using default server address and port." << std::endl;
		std::cout << "Port: " << server_port << std::endl;
		std::cout << "Server address: " << server_address << std::endl;
	}
	std::copy(filePath.begin(), filePath.end(), fileNameArray.begin());
	// Fill the remaining space with null characters if needed
	std::fill(fileNameArray.begin() + filePath.size(), fileNameArray.end(), '\0');
	/*---------------------------------------------------connecting to server---------------------------------------------------*/

	/*Initialize Winsock*/
	ConnectionHandler connectionHandler(server_address, server_port);
	connectionHandler.initialize();
	connectionHandler.connectToServer();


	/*---------------------------------------------------Sending connection requests---------------------------------------------------*/

	/* trying to read 'me' file contents, if fails try to register */
	if (FileReader::fileExists(meFileLocation) && FileReader::readMeFile(meFileLocation, client_name, hexRepresentation, publicKey)) {
		std::cout << "-------------'me' content copied successfully-------------" << std::endl;

		/* convert id hex to an ascii array<char> */
		user_id = hexToAscii(hexRepresentation);
		/* send  re-connection request */
		std::unique_ptr<Registration> reconnectRequest = std::make_unique <Registration>(user_id,
			version, RequestCode::CLIENT_RECONNECT, PAYLOAD_SIZE_RECONNECT, client_name);
		std::vector<char> requestBinary = reconnectRequest->serialize();

		if (connectionHandler.sendRequest(requestBinary)) {
			std::cout << "Reconnect request sent" << std::endl;
		}
		else {
			std::cout << "Error while sending reconnect request... aborting" << std::endl;
			return 1;
		}

	}
	/* try to register */
	else
	{
		std::cout << "attemping to register..." << std::endl;
		/* create a registration request and send it to server */
		std::unique_ptr<Request> request = RequestParser::createRequest(RequestCode::Register, client_name);
		std::vector<char> requestBinary = request->serialize();


		if (connectionHandler.sendRequest(requestBinary)) {
			std::cout << "<-----------------Register Request sent----------------->" << std::endl;

			std::cout << "Awaiting response..." << std::endl;
			int tries = 0;
			/* try to catch the response 3 times before giving up */
			bool receive_response = false;
			while (tries < 3 && !receive_response) {
				std::vector<char> requestBinary = connectionHandler.receiveRequest();

				if (!requestBinary.empty()) {
					receive_response = true;
					std::cout << "Request received" << std::endl;

					/* extract request code from basic request */
					auto basicData = RequestParser::unpackBasicServerRequest(requestBinary);
					RequestCode code = basicData.getCode();
					std::cout << "Request Code:" << static_cast<int>(code) << std::endl;

					switch (code) {
					case RequestCode::SERVER_REGISTER_OK: {
						std::cout << "Received SERVER_REGISTER_OK request" << std::endl;

						if (!user_ID) {
							user_ID = new UserID();
						}
						std::memcpy(user_ID, requestBinary.data() + requestBinary.size() - 16, sizeof(UserID));

						// Create an RSA decryptor.
						RSAPrivateWrapper rsapriv;

						//  get the public key
						publicKey = rsapriv.getPublicKey();
						std::cout << "Public key , length " << publicKey.length() << std::endl;
						keyHandler = new KeyHandler(publicKey);
						/* returns a base 64 key*/
						privateKey = keyHandler->RSA_generatePrivateKey();
						try {
							FileReader::saveStringAsFile(privateKey, "..\\Debug\\priv.key");
						}
						catch (...) {
							std::cout << " Error: Could not save private key " << std::endl;
						}


						std::memcpy(user_id.data(), user_ID->data, sizeof(user_ID->data));
						std::string user_id_string(user_id.begin(), user_id.end());
						std::stringstream hexStream;
						for (char c : user_id_string) {
							hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
						}
						std::string user_id_hex_string = hexStream.str();
						/* Create new 'me.info' file */
						
						std::string meFileInfo = client_name + "\n" + user_id_hex_string + "\n" + privateKey + "\0";
						try {
							FileReader::saveStringAsFile(meFileInfo, "..\\Debug\\me.info");
						}
						catch (...) {
							std::cout << " Error: Could not save me.info file " << std::endl;
						}
						
						/* Creating a request with client name and public key */
						std::unique_ptr<PublicKeyRequest> publicKeyRequest = std::make_unique <PublicKeyRequest>(user_id, version,
							RequestCode::CLIENT_PUBLIC_KEY, PAYLOAD_SIZE_REQUEST_1026, client_name + null_terminator, publicKey);
						std::vector<char> publicKeyRequest_Binary = publicKeyRequest->serialize();

						/* Sending request  */
						if (!connectionHandler.sendRequest(publicKeyRequest_Binary)) {
							std::cout << " Error: could not send public key " << std::endl;
						}
						else {
							std::cout << "<----------------------RSA key sent---------------------->" << std::endl;
						}
						break;
					}
					case RequestCode::SERVER_REGISTER_FAIL: {
						std::cout << "Received SERVER_REGISTER_FAIL, please change user name and try again." << std::endl;
						return 1;

						break;
					}
					} tries++;
				}
				if (tries == 3) {
					std::cout << "Error, did not receive a response" << std::endl;
					return 1;
				}
			}
		}
	}
	/*---------------------------------------------------Receive AES key and send encrypted file---------------------------------------------------*/

	std::cout << "\n<--------------------Public key Sent, awaiting response-------------------->" << std::endl;
	int answer_timer = 0;
	/* try to catch the response 3 times before giving up */
	while (answer_timer < 3) {
		/*expecting response 2102*/
		std::vector<char> server_response_binary = connectionHandler.receiveRequest();

		/* checking if response arrived */
		if (!server_response_binary.empty()) {
			answer_timer = 0;
			std::cout << "Response received " << std::endl;

			auto unpacked_request = RequestParser::unpackBasicServerRequest(server_response_binary);
			RequestCode code = unpacked_request.getCode();
			std::cout << "Request Code:" << static_cast<int>(code) << std::endl;

			if (code == RequestCode::SERVER_SEND_AES || code == RequestCode::SERVER_RECONNECT_AES) {

				/* Extract AES key from server message*/
				if (server_response_binary.size() <= 23) {
					std::cout << " binary file too small, unexpected error" << std::endl;
					return 1;
				}
				if (code == RequestCode::SERVER_RECONNECT_AES) {
					std::cout << "Reconnect successfull" << std::endl;
				}
				else if (code == RequestCode::SERVER_SEND_AES) {

				}
				/* get aes key */
				std::string aes_key_temp(server_response_binary.begin() + BASE_DATA_SIZE, server_response_binary.end());
				aes_key = aes_key_temp;
				std::cout << "------Aes key received------" << std::endl;

			}
			else if (code == RequestCode::SERVER_RECONNECT_DENIED) {
				std::cout << "Reconnection denied, please register. " << std::endl;
				return 1;
			}
			else {
				/* unknown request code*/
				std::cout << "Unknown error... aborting" << std::endl;
				return 1;
			}
		}
		else {
			std::cout << "ERROR: receiving public key failed " << std::endl;
			return 1;

		}
		break;
	}
	/*
	** This while loop sends a file and checks for the correct CRC, if wrong it attempts again up to three times.
	** if three attempts fail to get the correct CRC, abort
	*/
	int send_abort_message = 3;
	while (crc_sent < 4 && !file_sent) {
		unsigned char* converted_key = reinterpret_cast<unsigned char*>(const_cast<char*>(aes_key.data()));
		std::cout << "AES Key hex:" << std::endl;
		hexify(converted_key, strlen(reinterpret_cast<const char*>(converted_key)));
		/* Encrypt file string using AES key*/
		std::string file_string = FileReader::fileToString(filePath);
		const char* file_string_pointer = file_string.data();
		/* encrypt file */
		AESWrapper aes(converted_key, AESWrapper::DEFAULT_KEYLENGTH);
		std::string encryptedFile = aes.encrypt(file_string_pointer, file_string.length());
		const char* encryptedFileArray = encryptedFile.data();
		unsigned char* encryptedFileChar = reinterpret_cast<unsigned char*>(const_cast<char*>(encryptedFile.data()));
		unsigned int file_length = encryptedFile.size();
		int data_left_to_send = file_length;
		const size_t max_chunk_size = MAX_REQUEST_SIZE - BASE_DATA_SIZE;
		size_t offset = 0;
		std::cout << "encrypted file size:: " << file_length << std::endl;

		
		/*------------------------------------------Sending encrypted File header------------------------------------------*/
		size_t size = (data_left_to_send + BASE_DATA_SIZE > max_chunk_size) ? max_chunk_size : data_left_to_send;
		const char* chunk = encryptedFileArray + offset;
		std::string chunkData(chunk, size);

		std::unique_ptr<SendFileRequest> sendFileRequest = std::make_unique <SendFileRequest>(user_id, version,
			RequestCode::CLIENT_SEND_FILE,
			PAYLOAD_SIZE_REQUEST_1028, file_length, fileNameArray, chunkData);

		std::vector<char> sendFileRequestBinary = sendFileRequest->serialize();

		if (connectionHandler.sendRequest(sendFileRequestBinary)) {
			std::cout << "sending packet..." << std::endl;
		}
		else {
			std::cout << "ERROR sending packet, aborting" << std::endl;
			return 1;
		}
		data_left_to_send -= size;
		offset += size;

		/*------------------------------------Sending multiple packets without header----------------------------------------*/

		/* create requests with the encrypted file until all data was sent */
		while (data_left_to_send > 0) {
			// Calculate the size of the current chunk
			 size = (data_left_to_send + BASE_DATA_SIZE > max_chunk_size) ? max_chunk_size : data_left_to_send;

			const char* chunk = encryptedFileArray + offset;
			// Extract the current chunk
			std::string chunkData(chunk, size);

			std::vector<char> chunkVector(chunkData.begin(), chunkData.end());

			if (connectionHandler.sendRequest(chunkVector)) {
				std::cout << "sending packet..." << std::endl;
			}
			else {
				std::cout << "ERROR sending packet, aborting" << std::endl;
				return 1;
			}
			// Update the remaining data to be sent
			data_left_to_send -= size;
			offset += size;
			std::cout << "data left to send: " << data_left_to_send << std::endl;

		}
		checkSum = check_sum(filePath);


		/*---------------------------------------------------Wait for CRC response from server---------------------------------------------------*/
		int response_attempt = 0;
		while (response_attempt < 3) {
			std::cout << "Awaiting CRC from server  " << std::endl;
			std::vector<char> server_response_binary = connectionHandler.receiveRequest();

			/* if response received */
			if (!server_response_binary.empty()) {
				response_attempt = 0;
				std::cout << "Response received " << std::endl;

				/* unpack basic request and get the code*/
				const int CRC_LENGTH = 4;
				auto basicRequest = RequestParser::unpackBasicServerRequest(server_response_binary);
				auto code = basicRequest.getCode();
				if (code == RequestCode::SERVER_SENT_CRC) {

					/* get CRC */
					std::vector<char> CRC(server_response_binary.end() - CRC_LENGTH, server_response_binary.end());

					// Convert CRC vector to unsigned long
					unsigned long crc_value = 0;
					for (auto it = CRC.rbegin(); it != CRC.rend(); ++it) {
						crc_value = (crc_value << 8) | static_cast<unsigned char>(*it);
					}
					std::cout << "Client calculated CRC: " << checkSum << std::endl;
					if (crc_value == checkSum) {
						/* IF CRC OK */
						std::unique_ptr<ClientCRCRequest> CRC_ok_request = std::make_unique <ClientCRCRequest>(user_id, version,
							RequestCode::CLIENT_CRC_OK, PAYLOAD_SIZE_CRC, fileNameArray);
						std::vector<char> binaryRequest = CRC_ok_request->serialize();
						/* Send back a CRC Ok request, awaiting confirmation and shutting down */
						if (connectionHandler.sendRequest(binaryRequest)) {
							std::cout << "---------CRC OK - sent confirmation to server---------" << std::endl;
							file_sent = true;
							server_response_binary = connectionHandler.receiveRequest();
							if (!server_response_binary.empty()) {
								basicRequest = RequestParser::unpackBasicServerRequest(server_response_binary);
								code = basicRequest.getCode();

								if (code == RequestCode::SERVER_CONFIRM_RECEIVED) {
									std::cout << "Server received CRC OK message" << std::endl;
								}
								else {
									std::cout << "Server Error" << std::endl;
								}
							}
						}
						else {
							std::cout << "CRC ok, but there was an error while sending confirmation to server...\n"
								"retrying..." << crc_value << std::endl;
							crc_sent++;
						}
						break;
					} /* IF CRC NOT OK and not last attempt */
					else if (crc_sent < send_abort_message) {
						std::cout << "CRC MISMATCH - sending error to server " << crc_value << std::endl;
						std::unique_ptr<ClientCRCRequest> CRCfailedRequest = std::make_unique <ClientCRCRequest>(user_id, version,
							RequestCode::CLIENT_CRC_FAILED, PAYLOAD_SIZE_CRC, fileNameArray);
						auto CRCfailedRequestBinary = CRCfailedRequest->serialize();
						if (connectionHandler.sendRequest(CRCfailedRequestBinary)) {
							std::cout << "Sent CRC failed message" << crc_value << std::endl;

						}
						else {
							std::cout << "Error while sending CRC failed message " << crc_value << std::endl;
						}
						crc_sent++;
						break;
					}
					/* IF CRC NOT OK  and this is the last attemp*/
					else {
						std::cout << "CRC MISMATCH AFTER 3 ATTEMPTS - sending FINAL abort message. " << crc_value << std::endl;
						std::unique_ptr<ClientCRCRequest> CRCfailedRequest_FINAL = std::make_unique <ClientCRCRequest>(user_id, version,
							RequestCode::CLIENT_CRC_FAILED_FINAL, PAYLOAD_SIZE_CRC, fileNameArray);
						auto CRCfailedRequest_final_Binary = CRCfailedRequest_FINAL->serialize();

						if (connectionHandler.sendRequest(CRCfailedRequest_final_Binary)) {
							std::cout << "Sent CRC final failed message" << std::endl;
							/* Awaiting response and shutting down */
							server_response_binary = connectionHandler.receiveRequest();
							if (!server_response_binary.empty()) {
								basicRequest = RequestParser::unpackBasicServerRequest(server_response_binary);
								code = basicRequest.getCode();

								if (code == RequestCode::SERVER_CONFIRM_RECEIVED) {
									std::cout << "Server received CRC Failed message" << std::endl;
								}
								else {
									std::cout << "Server Error" << std::endl;
								}
							}
							else {
								std::cout << "Error while sending CRC final failed message " << crc_value << std::endl;
							}
							std::cout << "Aborting.... " << crc_value << std::endl;
							crc_sent++;
							break;

						}

					}
					/* send crc response */

				}

			}response_attempt++;
		}crc_sent++;
	}
	/*---------------------------------------------------Close connection---------------------------------------------------*/





	/* Close the socket*/
	connectionHandler.closeConnection();
	std::cout << "Connection closed." << std::endl;

	return 0;
}
