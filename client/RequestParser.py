from Request import *
from RequestHandler import *


class RequestParser:
    @staticmethod
    def parse_request(request, table_handler):
        RequestHandler.handle_request(request, table_handler)

    @staticmethod
    def unpack_request(packed_data):
        """
        Parse the packed data and determine the appropriate class for deserialization.

        Args:
            packed_data (bytes): The binary data to parse.

        Returns:
            Request: An instance of the appropriate Request subclass based on the code.
        """
        # Use struct to unpack the common data to get the request code
        basic_request = Request.deserialize(packed_data)
        code = basic_request.get_code()

        # Determine the appropriate class based on the request code
        if code == RequestCode.REGISTRATION:
            return ConnectionRequest.deserialize(packed_data)
        if code == RequestCode.CLIENT_PUBLIC_KEY:
            return ClientPublicKeyRequest.deserialize(packed_data)
        if code == RequestCode.CLIENT_SEND_FILE:
            return FileTransferRequest.deserialize(packed_data)
        if code == RequestCode.CLIENT_CRC_OK:
            return Client_CRC_Response.deserialize(packed_data)
        if code == RequestCode.CLIENT_CRC_FAILED:
            return Client_CRC_Response.deserialize(packed_data)
        if code == RequestCode.CLIENT_CRC_FAILED_FINAL:
            return Client_CRC_Response.deserialize(packed_data)
        if code == RequestCode.CLIENT_RECONNECT:
            return ConnectionRequest.deserialize(packed_data)
        # Add more conditions for other request codes as needed
        else:
            # Handle unknown or unsupported request codes
            print(f"Unknown request code: {basic_request.code}")
