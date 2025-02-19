import sqlite3


# Query data from the table
# cursor.execute("SELECT * FROM clients")
# result = cursor.fetchall()
# for row in result:
#    print(row)
# cursor.execute("SELECT * FROM files")
# result = cursor.fetchall()
# for row in result:
#    print(row)


class TableHandler:
    def __init__(self, connection, lock):
        # Create a cursor object to execute SQL commands
        self.conn = connection
        self.cursor = self.conn.cursor()
        self.lock = lock

    def insert_client(self, request):
        # Insert data into the clients table
        with self.lock:
            exist = self.query_is_exist(request)
            if not exist:
                self.cursor.execute("INSERT INTO clients (ID, Name, LastSeen) VALUES (?, ?, ?)"
                                    , request)
                self.conn.commit()

    def insert_client_keys(self, request):
        # insert keys into client table
        with self.lock:
            exist = self.query_is_exist(request)
            if not exist:
                self.cursor.execute("UPDATE clients SET PublicKey = ?, LastSeen = ?, AES_Key = ? WHERE ID = ?",
                                    request)
                self.conn.commit()

    def update_request_time(self, request):
        # update the last request in the client table
        with self.lock:
            self.cursor.execute("UPDATE clients SET LastSeen = ? WHERE ID = ?", request)

    def remove_files(self, user_id):
        with self.lock:
            self.cursor.execute("""
                UPDATE files
                SET File_Name = NULL, Path_Name = NULL, Verified = NULL
                WHERE ID = ?
            """, (user_id,))
            self.conn.commit()

    def insert_files(self, request):
        # Insert data into the files table
        with self.lock:
            self.cursor.execute("""
                            INSERT OR REPLACE INTO files (ID, File_Name, Path_Name, Verified)
                            VALUES (?, ?, ?, ?)
                        """, request)
            self.conn.commit()

    def update_verified_file(self, request):
        with self.lock:
            self.cursor.execute("UPDATE files SET Verified = ? WHERE ID = ?", request)

    def query_is_exist(self, request):
        duo_request = (request[0], request[1])
        self.cursor.execute("SELECT * FROM clients WHERE ID = ? AND Name = ?", duo_request)
        existing_client = self.cursor.fetchone()
        return existing_client

    def query_is_name_exist(self, name):
        self.cursor.execute("SELECT * FROM clients WHERE Name = ?", (name,))
        existing_client = self.cursor.fetchone()
        return existing_client

    def query_is_id_exists(self, user_id):
        self.cursor.execute("SELECT * FROM clients WHERE ID = ?", (user_id,))
        existing_client = self.cursor.fetchone()
        return existing_client

    def is_public_key_exist(self, user_id):
        self.cursor.execute("SELECT PublicKey FROM clients WHERE ID = ?", (user_id,))
        existing_key = self.cursor.fetchone()
        return existing_key
    def print_all(self):
        self.print_clients_table()
        self.print_files_table()
        print("\n")

    def print_clients_table(self):
        print("- - - clients table - - -")
        self.cursor.execute("SELECT * FROM clients")
        clients = self.cursor.fetchall()
        for client in clients:
            print(client)

    def print_files_table(self):
        print("- - - files table  - - -")
        self.cursor.execute("SELECT * FROM files")
        files = self.cursor.fetchall()
        for file in files:
            print(file)

    def get_client_id_by_name(self, user_name):
        with self.lock:
            self.cursor.execute("SELECT ID from clients where Name = ?", (user_name,))
            user_id = self.cursor.fetchone()
            return user_id if user_id else None

    def get_aes_key(self, user_id):
        # Retrieve the AES key for a specific user ID
        with self.lock:
            self.cursor.execute("SELECT AES_Key FROM clients WHERE ID = ?", (user_id,))
            aes_key = self.cursor.fetchone()
            return aes_key if aes_key else None
