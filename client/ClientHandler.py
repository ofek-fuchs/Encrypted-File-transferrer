from RequestParser import *


def handle_client(client_socket, table_handler):
    try:
        i = 0
        while True:
            i += 1
            binary_request = client_socket.recv(4096)
            if not binary_request:
                print("Client disconnected")
                break
            print("Request received")
            request = RequestParser.unpack_request(binary_request)
            request_code = request.get_code()
            incomplete_binary_data = binary_request

            # request is a file transfer wait until all the data arrived
            if request_code == RequestCode.CLIENT_SEND_FILE:
                content_size = request.get_content_size()

                while len(incomplete_binary_data) < content_size + Data.dataSize + RequestPayloadSize.CLIENT_SEND_FILE:
                    more_data = client_socket.recv(4096)
                    incomplete_binary_data += more_data[:]
                request = RequestParser.unpack_request(incomplete_binary_data)

            print("unpack Request info:")
            server_response = RequestHandler.handle_request(request, table_handler)
            print("--------------current tables--------------")
            table_handler.print_all()

            if server_response is not None:
                print("Server response made, Request code:", server_response.get_code())
                binary_data = server_response.serialize()
                print("Binary serialized")
                client_socket.sendall(binary_data)
                print("Binary sent")

    except Exception as e:
        print(f"Error handling client: {e}")

    finally:
        print("closing client socket...")
        client_socket.close()
