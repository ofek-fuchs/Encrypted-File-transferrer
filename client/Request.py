import struct

from enum import IntEnum


class RequestPayloadSize(IntEnum):
    REGISTRATION = 255
    CLIENT_RECONNECT = 255
    CLIENT_CRC_OK = 255
    CLIENT_CRC_FAILED = 255
    CLIENT_CRC_FAILED_FINAL = 255
    CLIENT_PUBLIC_KEY = 415
    # without file content = 259
    CLIENT_SEND_FILE = 259

    # outgoing Request
    SERVER_REGISTER_OK = 16
    SERVER_CONFIRM_RECEIVED = 16
    SERVER_RECONNECT_DENIED = 16
    SERVER_SEND_CRC = 279
    SERVER_UNKNOWN_ERROR = 0
    SERVER_REGISTER_FAIL = 0
    # SERVER_SEND_AES -> varies
    # SERVER_RECONNECT_AES -> varies


class RequestCode(IntEnum):
    # Define your request codes as needed
    # incoming Requests
    REGISTRATION = 1025
    CLIENT_PUBLIC_KEY = 1026
    CLIENT_RECONNECT = 1027
    CLIENT_SEND_FILE = 1028
    CLIENT_CRC_OK = 1029
    CLIENT_CRC_FAILED = 1030
    CLIENT_CRC_FAILED_FINAL = 1031

    # outgoing Request
    SERVER_REGISTER_OK = 2100
    SERVER_REGISTER_FAIL = 2101
    SERVER_SEND_AES = 2102
    SERVER_SEND_CRC = 2103
    SERVER_CONFIRM_RECEIVED = 2104
    SERVER_RECONNECT_AES = 2105
    SERVER_RECONNECT_DENIED = 2106
    SERVER_UNKNOWN_ERROR = 2107


content_byte_size = 4
filename_byte_size = 255


class Data:
    dataSize = 23

    def __init__(self, client_id, version, code, payload_size):
        self.client_id = client_id
        self.version = version
        self.code = code
        self.payload_size = payload_size

    def pack(self):
        return struct.pack('<16sBHI', self.client_id, self.version, self.code, self.payload_size)

    @classmethod
    def unpack(cls, packaged_data):
        # Unpack data and print for debugging
        unpacked_data = struct.unpack('<16sBHI', packaged_data[:cls.dataSize])

        # Return an instance of the Data class with the unpacked data
        return cls(*unpacked_data)


class ServerData:

    def __init__(self, version, code, payload_size):
        self.version = version
        self.code = code
        self.payload_size = payload_size

    def pack(self):
        return struct.pack('<BHI', self.version, self.code, self.payload_size)

    @classmethod
    def unpack(cls, packed_data):
        unpacked_data = struct.unpack('<BHI', packed_data[:])

        return cls(*unpacked_data)


class ServerRequest:
    def __init__(self, version, code, payload_size):
        self.data = ServerData(version, code, payload_size)

    @classmethod
    def deserialize(cls, packed_data):
        common_request = ServerData.unpack(packed_data[:])
        return cls(version=common_request.version,
                   code=common_request.code,
                   payload_size=common_request.payload_size)

    def serialize(self):
        return self.data.pack()

    def get_code(self):
        return self.data.code

    def get_version(self):
        return self.data.version

    def get_payload_size(self):
        return self.data.payload_size


class Request:

    def __init__(self, client_id, version, code, payload_size):
        """
        Initialize the Request object with common request data.

        Args:
            client_id (bytes): The client ID (16 bytes).
            version (int): The version number (1 byte).
            code (int): The request code (2 bytes).
            payload_size (int): The size of the payload (4 bytes).
        """
        # Initialize the Data object with common request data
        self.data = Data(client_id, version, code, payload_size)

    @classmethod
    def deserialize(cls, packed_data):
        # Use Data.unpack to extract common data
        common_request = Data.unpack(packed_data[:Data.dataSize])

        # Create a Request instance using the extracted data
        return cls(
            client_id=common_request.client_id.decode('utf-8'),
            version=common_request.version,
            code=common_request.code,
            payload_size=common_request.payload_size
        )

    def serialize(self):
        return self.data.pack()

    def get_code(self):
        return self.data.code

    def get_client_id(self):
        return self.data.client_id

    def get_payload_size(self):
        return self.data.payload_size

    def get_version(self):
        return self.data.version

    def print_info(self):
        """
        Print information about the Request.
        """
        print("Request Information:")
        print(f"Client ID: {self.data.client_id}")
        print(f"Version: {self.data.version}")
        print(f"Request Code: {self.data.code}")
        print(f"Payload Size: {self.data.payload_size}")


# request 1026
class ClientPublicKeyRequest(Request):
    def __init__(self, client_id, version, code, payload_size, name, public_key):
        super().__init__(client_id, version, code, payload_size)
        self.name = name
        self.public_key = public_key  # Use consistent attribute name here

    @classmethod
    def deserialize(cls, packed_data):
        try:

            common_request = Data.unpack(packed_data[:Data.dataSize])
            null_char_index = packed_data.find(b'/')
            name_data = packed_data[Data.dataSize:null_char_index].decode('utf-8')
            public_key_packed = packed_data[null_char_index + 1:]

            # Use the consistent attribute name when instantiating the class
            return cls(
                client_id=common_request.client_id.decode('utf-8'),
                version=common_request.version,
                code=common_request.code,
                payload_size=common_request.payload_size,
                name=name_data,
                public_key=public_key_packed
            )
        except Exception as e:
            print(f"Error during deserialization: {e}")

    def get_name(self):
        return self.name

    def get_public_key(self):
        return self.public_key


# request 2100 and 2104
class ServerRequestUserID(ServerRequest):
    def __init__(self, version, code, payload_size, payload):
        super().__init__(version, code, payload_size)
        self.payload = payload

    def serialize(self):
        # Assuming that final_id is a string
        payload_bytes = self.payload.encode('utf-8') if isinstance(self.payload, str) else self.payload
        packaged_data = struct.pack('<BHI16s', self.get_version(), self.get_code(), self.get_payload_size(),
                                    payload_bytes)
        return packaged_data


# request 2102 and 2105
class AESkey_ServerRequest(ServerRequest):
    def __init__(self, client_id, version, code, payload_size, aes_key):
        super().__init__(version, code, payload_size)
        self.client_id = client_id
        self.aes_key = aes_key

    def serialize(self):
        # Encode client_id and aes_key as bytes
        client_id_bytes = self.client_id.encode('utf-8') if isinstance(self.client_id, str) else self.client_id
        aes_key_bytes = self.aes_key.encode('utf-8') if isinstance(self.aes_key, str) else self.aes_key

        # Calculate payload size dynamically based on the size of client_id and aes_key
        payload_size = len(client_id_bytes) + len(aes_key_bytes)

        # Pack the data with a variable-sized payload
        packaged_data = struct.pack(f'<BHI{len(client_id_bytes)}s{len(aes_key_bytes)}s',
                                    self.get_version(), self.get_code(), payload_size,
                                    client_id_bytes, aes_key_bytes)

        return packaged_data


# request 2103
class CRC_ServerRequest(ServerRequest):
    def __init__(self, version, code, payload_size, client_id, content_size, file_name, checksum):
        super().__init__(version, code, payload_size)
        self.client_id = client_id
        self.content_size = content_size
        self.file_name = file_name
        self.checksum = checksum

    def serialize(self):
        base_data = super().serialize()
        # Pack additional data
        additional_data = struct.pack('<16s I 255s L', self.client_id.encode('utf-8'),
                                      self.content_size,
                                      self.file_name.encode('utf-8'),
                                      self.checksum)
        checksumBinary = struct.pack('<L', self.checksum)
        # Concatenate base data and additional data
        packaged_data = base_data + additional_data

        return packaged_data


# request 1029,1030,1031
class Client_CRC_Response(Request):
    def __init__(self, client_id, version, code, payload_size, file_name):
        super().__init__(client_id, version, code, payload_size)
        self.file_name = file_name

    def get_file_name(self):
        return self.file_name

    @classmethod
    def deserialize(cls, packed_data):
        common_request = Data.unpack(packed_data[:Data.dataSize])
        file_name = packed_data[Data.dataSize:]
        file_name = file_name.decode('utf-8').rstrip('\0')
        return cls(
            client_id=common_request.client_id,
            version=common_request.version,
            code=common_request.code,
            payload_size=common_request.payload_size,
            file_name=file_name
        )


# request 1025 and 1027
class ConnectionRequest(Request):
    def __init__(self, client_id, version, code, payload_size, name):
        """
        Initialize the RegistrationRequest object with common and specific data.

        Args:
            client_id (bytes): The client ID (16 bytes).
            version (int): The version number (1 byte).
            code (int): The request code (2 bytes).
            payload_size (int): The size of the payload (4 bytes).
            name (str): The name for the registration request.
        """
        # Initialize the common request data using the base class method
        super().__init__(client_id, version, code, payload_size)
        # Add additional data specific to RegistrationRequest
        self.name = name

    def get_name(self):
        return self.name

    @classmethod
    def deserialize(cls, packed_data):
        """
        Deserialize packed data into a RegistrationRequest object.

        Args:
            packed_data (bytes): The binary data to deserialize.

        Returns:
            RegistrationRequest: An instance of the RegistrationRequest class.
                                Returns None if the code doesn't indicate a registration request.
        """
        try:
            # Use struct to unpack the common data
            common_request = Data.unpack(packed_data[:Data.dataSize])
            # Decode the bytes to a string using the appropriate encoding (e.g., utf-8)
            name_data = packed_data[Data.dataSize:].decode('utf-8')

            # Return the instance if deserialization is successful
            return cls(
                client_id=common_request.client_id,
                version=common_request.version,
                code=common_request.code,
                payload_size=common_request.payload_size,
                name=name_data
            )
        except Exception as e:
            # Handle the exception (you can print an error message if needed)
            print(f"Error during deserialization: {e}")

    def print_info(self):
        """
        Print information about the RegistrationRequest.
        """
        # Call the print_info method from the base class to print common request information
        super().print_info()
        # Add specific information for RegistrationRequest
        print(f"Name: {self.name}")


# request 1028
class FileTransferRequest(Request):
    def __init__(self, client_id, version, code, payload_size, content_size, file_name, file_content):
        super().__init__(client_id, version, code, payload_size)

        self.content_size = content_size
        self.file_name = file_name
        self.file_content = file_content

    def get_file_content(self):
        return self.file_content

    def get_file_name(self):
        return self.file_name

    def get_content_size(self):
        return self.content_size

    @classmethod
    def deserialize(cls, packed_data):
        common_request = Data.unpack(packed_data[: Data.dataSize])
        content_size = struct.unpack('<I', packed_data[Data.dataSize: Data.dataSize + content_byte_size])[0]
        file_name = struct.unpack('<255s ', packed_data[Data.dataSize + content_byte_size:
                                                        Data.dataSize + content_byte_size
                                                        + filename_byte_size])[0].decode('utf-8').rstrip('\0')
        file_content = packed_data[Data.dataSize + content_byte_size + filename_byte_size:]

        # Return the instance if deserialization is successful
        return cls(
            client_id=common_request.client_id.decode('utf-8'),
            version=common_request.version,
            code=common_request.code,
            payload_size=common_request.payload_size,
            content_size=content_size,
            file_name=file_name,
            file_content=file_content
        )
