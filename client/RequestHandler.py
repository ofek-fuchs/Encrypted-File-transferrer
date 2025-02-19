import os

from Crypto.Util.Padding import unpad
from cksum import *
from Request import *
from Crypto.Cipher import AES
import secrets
import datetime

server_version = 1


# request handler class is responsible for dealing with the different actions required
# by the requests
class RequestHandler:

    @staticmethod
    def handle_request(request, table_handler):
        request_code = Request.get_code(request)

        # Define cases using a dictionary
        cases = {
            RequestCode.REGISTRATION: RequestHandler.handle_registration,
            RequestCode.CLIENT_PUBLIC_KEY: RequestHandler.handle_public_key,
            RequestCode.CLIENT_SEND_FILE: RequestHandler.incoming_file_handler,
            RequestCode.CLIENT_CRC_OK: RequestHandler.crc_ok_received,
            RequestCode.CLIENT_CRC_FAILED: RequestHandler.crc_fail,
            RequestCode.CLIENT_CRC_FAILED_FINAL: RequestHandler.crc_failed_final,
            RequestCode.CLIENT_RECONNECT: RequestHandler.handle_reconnection
        }

        # Get the handler function based on the request code
        handler = cases.get(request_code, RequestHandler.default_handler)

        # Call the handler function
        response = handler(request, table_handler)
        return response

    @staticmethod
    def generate_id():
        temp_id = secrets.token_bytes(8)
        user_id = temp_id.hex()
        return user_id

    # handle reconnection requests
    @staticmethod
    def handle_reconnection(request, table_handler):
        print("Checking if client exists...")
        client_name = request.get_name()
        # check if client name exists in db
        table_handler.print_clients_table()
        if table_handler.query_is_name_exist(client_name):
            print("Client name found, locating key")
            client_id = table_handler.get_client_id_by_name(client_name)[0]
            print("client id type: ", type(client_id))
            # check if client has a public key generated already
            if table_handler.is_public_key_exist(client_id):
                AES_key = table_handler.get_aes_key(client_id)[0]
                # reconnect approved
                return AESkey_ServerRequest(client_id, server_version, RequestCode.SERVER_RECONNECT_AES,
                                            RequestPayloadSize.SERVER_REGISTER_OK + AES_key.__sizeof__(), AES_key)
                # reconnect denied
            else:
                print("Client keys not found")
                return ServerRequestUserID(request.get_version(), RequestCode.SERVER_RECONNECT_DENIED,
                                           RequestPayloadSize.SERVER_RECONNECT_DENIED, client_id)

        else:
            print("Client name not found")
            return ServerRequest(request.get_version(), RequestCode.SERVER_RECONNECT_DENIED,
                                 RequestPayloadSize.SERVER_RECONNECT_DENIED)

    # handle a 4th and final crc failed message from client
    @staticmethod
    def crc_failed_final(request, table_handler):
        print("CRC last request failed, aborting...")
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        client_id = request.get_client_id()
        update_client_last_request = (current_time, client_id)
        # update files table and lastSeen in clients table
        table_handler.remove_files(client_id)
        table_handler.update_request_time(update_client_last_request)
        file_name = request.get_file_name()
        file_path = f"output/{file_name}"

        if os.path.exists(file_path):
            try:
                os.remove(file_path)
                print(f"File '{file_path}' deleted successfully.")
            except OSError as e:
                print(f"Error deleting file '{file_path}': {e}")
        else:
            print(f"File '{file_path}' does not exist.")

        return ServerRequestUserID(request.get_version(), RequestCode.SERVER_CONFIRM_RECEIVED,
                                   RequestPayloadSize.SERVER_CONFIRM_RECEIVED, client_id)

    # handle a crc failed message from the client
    @staticmethod
    def crc_fail(request, table_handler):
        print("........................................"
              "\nCRC request Failed, awaiting file")
        client_id = request.get_client_id()
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        update_client_last_request = (current_time, request.get_client_id())
        # update files table and lastSeen in clients table
        table_handler.update_request_time(update_client_last_request)
        table_handler.remove_files(client_id)
        file_name = request.get_file_name()
        file_path = f"output/{file_name}"

        if os.path.exists(file_path):
            try:
                os.remove(file_path)
                print(f"File '{file_path}' deleted successfully.")
            except OSError as e:
                print(f"Error deleting file '{file_path}': {e}")
        else:
            print(f"File '{file_path}' does not exist.")
        return None

# handle crc ok received from client
    @staticmethod
    def crc_ok_received(request, table_handler):
        client_id = request.get_client_id()
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        verifyFileRequest = (1, request.get_client_id())
        update_client_last_request = (current_time, request.get_client_id())
        # update verified in files table and lastSeen in clients table
        table_handler.update_verified_file(verifyFileRequest)
        table_handler.update_request_time(update_client_last_request)
        print("File verified successfully")
        return ServerRequestUserID(request.get_version(), RequestCode.SERVER_CONFIRM_RECEIVED,
                                   RequestPayloadSize.SERVER_CONFIRM_RECEIVED, client_id)

# handle client's initial registration
    @staticmethod
    def handle_registration(request, table_handler):
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        # check if name exists in db
        if not table_handler.query_is_name_exist(request.get_name()):
            check_id = RequestHandler.generate_id()
            # if id exists keep generating until you find an available one or timeout
            while table_handler.query_is_id_exists(check_id):
                check_id = RequestHandler.generate_id()
            final_id = check_id
            insert_tuple = (check_id, request.get_name(), current_time)
            table_handler.insert_client(insert_tuple)
            print("Client inserted to client table")
            table_handler.print_clients_table()
            return ServerRequestUserID(request.get_version(), RequestCode.SERVER_REGISTER_OK,
                                       RequestPayloadSize.SERVER_REGISTER_OK, final_id)
        else:
            return ServerRequest(server_version, RequestCode.SERVER_REGISTER_FAIL,
                                 RequestPayloadSize.SERVER_REGISTER_FAIL)

# handle receiving public keys from client
    @staticmethod
    def handle_public_key(request: ClientPublicKeyRequest, table_handler):
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        print("RSA key received, sending AES")
        RSA_key = request.get_public_key()
        AES_key = RequestHandler.generate_aes_key(RSA_key)
        # encrypt aes key using public RSA key
        client_id = request.get_client_id()
        DB_request = (RSA_key, current_time, AES_key, client_id)
        table_handler.insert_client_keys(DB_request)
        print("Keys inserted to client table")
        return AESkey_ServerRequest(client_id, server_version, RequestCode.SERVER_SEND_AES,
                                    RequestPayloadSize.SERVER_REGISTER_OK + AES_key.__sizeof__(), AES_key)

# handle unknown error code
    @staticmethod
    def default_handler(request):

        print("ERROR: UNKNOWN SERVER ERROR")

# handle incoming encrypted file
    @staticmethod
    def incoming_file_handler(request, table_handler):
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        file_content = request.get_file_content()
        file_name = request.get_file_name()
        client_id = request.get_client_id()
        request_size = request.get_content_size()
        file_path = ""
        aes_key = table_handler.get_aes_key(request.get_client_id())[0]
        iv = bytes(b'\x00' * 16)
        print("Aes key hex: ", aes_key.hex())
        cipher = AES.new(aes_key, AES.MODE_CBC, iv)
        print("File content length before decryption:", len(file_content))
        check_sum = 0

        try:
            print("Coded file received, Decrypting...")
            decrypted_file_content = cipher.decrypt(file_content)
            unpadded_file_content = unpad(decrypted_file_content, AES.block_size).decode('utf-8')
            unpadded_file_content = unpadded_file_content.replace('\r\n', '\n')
            print(unpadded_file_content)

            file_path = f"output/{file_name}"
            os.makedirs(os.path.dirname(file_path), exist_ok=True)
            with open(file_path, 'w') as output_file:
                output_file.write(unpadded_file_content)
            check_sum = readfile(file_path)
            print("File decrypted successfully, CRC:", check_sum)

        except Exception as e:
            print("Error during decryption:", e)

        file_request = (client_id, file_name, file_path, 0)
        update_time_request = (current_time, client_id)
        table_handler.update_request_time(update_time_request)
        table_handler.insert_files(file_request)
        return CRC_ServerRequest(server_version, RequestCode.SERVER_SEND_CRC,
                                 RequestPayloadSize.SERVER_SEND_CRC, client_id, request_size, file_name, check_sum)

# generate a new aes key from a client's key
    @staticmethod
    def generate_aes_key(master_key):
        # Ensure the master key is 16, 24, or 32 bytes long (AES-128, AES-192, or AES-256)
        key_length = len(master_key)
        if key_length < 16:
            # Pad with zeros if the key is too short
            master_key = master_key.ljust(16, b'\0')
        elif key_length > 16:
            # Truncate if the key is too long
            master_key = master_key[:16]

        # Generate a random initialization vector (IV)
        iv = bytes([0] * 16)

        # Create an AES cipher object with the master key and AES.MODE_CBC mode
        cipher = AES.new(master_key, AES.MODE_CBC, iv)

        # Generate the AES key by encrypting a block of zeros
        aes_key = cipher.encrypt(bytes(16))
        return aes_key
