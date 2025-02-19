import socket
import threading
import os
import sqlite3

from ClientHandler import *
from TableHandler import *

port_path = "port.info"
default_port = 1357


# Main function
def main():
    # Insert data into the table
    # cursor.execute("INSERT INTO clients (ID, Name) VALUES (?, ?)", ('newestestest', 'john@example.com'))
    # Commit the changes
    # conn.commit()

    # Read the port number from the "port.info" file
    if os.path.isfile(port_path):
        with open(port_path, "r") as port_file:
            port = int(port_file.read().strip())
    else:
        print(f"Warning: missing file 'port.info'. default port used {default_port}.")
        port = int(default_port)

    # Create a socket object
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the server to the specified port
    server.bind(('0.0.0.0', port))

    # Connect to an SQLite database or create it if it doesn't exist
    conn = sqlite3.connect('defensive.db', check_same_thread=False)

    cursor = conn.cursor()
    cursor.execute('''CREATE TABLE IF NOT EXISTS clients (
                        ID BLOB PRIMARY KEY,
                        Name TEXT,
                        PublicKey BLOB,
                        LastSeen TEXT,
                        AES_Key BLOB)
                    ''')

    cursor.execute('''CREATE TABLE IF NOT EXISTS files (
                        ID BLOB PRIMARY KEY,
                        File_Name TEXT,
                        Path_name TEXT,
                        Verified INTEGER CHECK (Verified IN (0, 1)))
                    ''')
    # Commit the changes
    conn.commit()

    lock = threading.Lock()
    # create object for handling table actions
    table_handler = TableHandler(conn, lock)


    # Start listening for incoming connections
    server.listen(5)
    print(f"Server is listening on port {port}...")
    try:
        while True:
            # Accept a client connection
            client_socket, addr = server.accept()
            print(f"Accepted connection from {addr[0]}:{addr[1]}")

            # Create a new thread to handle the client
            client_handler = threading.Thread(target=handle_client, args=(client_socket, table_handler))
            client_handler.start()

    except KeyboardInterrupt:
        # Handle manual termination (Ctrl+C)
        print("Shutting down the server...")
    # Close the database connection
    conn.close()
    # Close the server socket
    server.close()


if __name__ == "__main__":
    main()
