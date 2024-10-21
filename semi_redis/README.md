### Project Overview

**Semi-Redis** is a lightweight, simplified version of Redis, implemented in C++. The project uses socket programming to handle client-server communication, allowing clients to store and retrieve key-value pairs across multiple databases. This project demonstrates basic Redis-like functionality using multi-threading and simple persistence mechanisms.

<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/en/6/6b/Redis_Logo.svg" alt="Redis logo" width="300">
</p>

Redis is a powerful, open-source, in-memory data structure store, commonly used as a distributed, in-memory key-value database, cache, and message broker. You can find more details on the official Redis website: [Redis.io](https://redis.io).

---

### How Redis Works

Redis is built around a key-value model. Clients can set and retrieve values using keys. The data is stored in memory for fast access and optionally persisted to disk. Redis operations are atomic, ensuring consistency in concurrent environments. It supports both simple key-value pairs and more complex data types (e.g., lists, sets).

---

### How This Project Implements Redis

This project mimics the core functionalities of Redis in a simplified manner:

1. **Client-Server Architecture**: Clients communicate with the server using TCP sockets. The server listens for commands from clients, processes them, and returns responses.
   
2. **Key-Value Storage**: Each database stores key-value pairs in a map data structure (`std::map`). Clients can store and retrieve data by interacting with these maps.
   
3. **Multi-Database Support**: The server allows clients to select between different databases (`DB1`, `DB2`, etc.), similar to how Redis supports multiple databases.
   
4. **Persistence**: The project includes basic data persistence. The server saves the current state to a file when requested and can load this state back upon startup.

5. **Threaded Operations**: To handle multiple clients simultaneously, the server spawns a new thread for each client connection. This ensures non-blocking operations and simulates the asynchronous nature of Redis.

6. **Command Set**:
   - `add <key> <value>`: Add a new key-value pair to the current database.
   - `get <key>`: Retrieve the value associated with a key.
   - `select <db>`: Switch to a different database.
   - `save`: Save the current state of the database to a file.
   - `load`: Load the database state from a file.

---

### How to Build and Run the Project

1. **Clone the repository**.
2. **Build the project** using CMake.
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
3. **Run the server**:
   ```bash
   ./semi_redis
   ```
4. **Run a client**:
   ```bash
   ./client
   ```

Now you can start sending commands from the client to interact with the server.

### **Interacting with the Server:**
   You can now input commands into the client. Here are the commands you can use:

   - **Add a Key-Value Pair:**
     ```
     add <key> <value>
     ```
     Example:
     ```
     add mykey myvalue
     ```
     This will store the key-value pair in the currently selected database.

   - **Get the Value of a Key:**
     ```
     get <key>
     ```
     Example:
     ```
     get mykey
     ```
     This retrieves the value associated with the key `mykey`.

   - **Select a Database:**
     ```
     select <db_number>
     ```
     Example:
     ```
     select 1
     ```
     This switches to database number `1`. The default is database `0`.

   - **Save the Database:**
     ```
     save
     ```
     This saves the current state of the databases to a `database.json` file.

   - **Load the Database:**
     ```
     load
     ```
     This loads the saved state from the `database.json` file.

   - **Exit:**
     To exit the client, you can simply type:
     ```
     exit
     ```