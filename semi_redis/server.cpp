#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <map>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;

#define PORT 8080

mutex myMutex;
int server_fd, new_socket;
vector<map<string, string>> container(2);  // Two databases
int dbNumber = 0;

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    void enqueueJob(function<void()> job);
private:
    vector<thread> workers;
    queue<function<void()>> jobs;
    mutex queueMutex;
    condition_variable condition;
    bool stop;
    void workerThread();
};

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] { workerThread(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        unique_lock<mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (thread &worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueueJob(function<void()> job) {
    {
        unique_lock<mutex> lock(queueMutex);
        jobs.push(job);
    }
    condition.notify_one();
}

void ThreadPool::workerThread() {
    while (true) {
        function<void()> job;
        {
            unique_lock<mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !jobs.empty() || stop; });
            if (stop && jobs.empty()) return;
            job = move(jobs.front());
            jobs.pop();
        }
        job();
    }
}

// Helper functions for command processing
void processCommand(string command, int client_socket);
void add(string key, string value, int client_socket);
void getValue(string key, int client_socket);
void selectDB(int db, int client_socket);
void save(int client_socket);
void load(int client_socket);

int main() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    ThreadPool pool(4);  // Use a thread pool with 4 threads

    // Socket setup
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        cout << "+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        long valread = read(new_socket, buffer, 30000);
        string command(buffer);

        // Pass the client socket (new_socket) to processCommand
        pool.enqueueJob([command, new_socket]() { processCommand(command, new_socket); });

        // We should not close the socket immediately, leave it open until the client has finished
    }

    return 0;
}


void processCommand(string command, int client_socket) {
    istringstream iss(command);
    string cmd, key, value;
    iss >> cmd;

    if (cmd == "add") {
        iss >> key >> value;
        add(key, value, client_socket);  // Pass the client socket
    } else if (cmd == "get") {
        iss >> key;
        getValue(key, client_socket);    // Pass the client socket
    } else if (cmd == "select") {
        int db;
        iss >> db;
        selectDB(db, client_socket);     // Pass the client socket
    } else if (cmd == "save") {
        save(client_socket);             // Pass the client socket
    } else if (cmd == "load") {
        load(client_socket);             // Pass the client socket
    } else {
        string errMsg = "Invalid command\n";
        write(client_socket, errMsg.c_str(), errMsg.size());  // Send error message back to the client
    }
}

void add(string key, string value, int client_socket) {
    lock_guard<mutex> guard(myMutex);
    if (container[dbNumber].find(key) == container[dbNumber].end()) {
        container[dbNumber][key] = value;
        string msg = "Key added successfully\n";
        write(client_socket, msg.c_str(), msg.size());  // Send success message back to the client
    } else {
        string errMsg = "Key already exists\n";
        write(client_socket, errMsg.c_str(), errMsg.size());  // Send error message back to the client
    }
}

void getValue(string key, int client_socket) {
    lock_guard<mutex> guard(myMutex);
    if (container[dbNumber].find(key) != container[dbNumber].end()) {
        string value = container[dbNumber][key] + "\n";
        write(client_socket, value.c_str(), value.size());  // Send value back to the client
    } else {
        string errMsg = "Key does not exist\n";
        write(client_socket, errMsg.c_str(), errMsg.size());  // Send error message back to the client
    }
}

void selectDB(int db, int client_socket) {
    lock_guard<mutex> guard(myMutex);
    if (db >= 0 && db < container.size()) {
        dbNumber = db;
        string msg = "Database selected\n";
        write(client_socket, msg.c_str(), msg.size());  // Send confirmation message back to the client
    } else {
        string errMsg = "Invalid database number\n";
        write(client_socket, errMsg.c_str(), errMsg.size());  // Send error message back to the client
    }
}

void save(int client_socket) {
    lock_guard<mutex> guard(myMutex);
    ofstream outFile("database.json");
    outFile << "{";
    for (int i = 0; i < container.size(); ++i) {
        outFile << "\"DB" << i << "\": {";
        for (auto& pair : container[i]) {
            outFile << "\"" << pair.first << "\": \"" << pair.second << "\", ";
        }
        outFile << "}, ";
    }
    outFile << "}";
    outFile.close();
    string msg = "Database saved\n";
    write(client_socket, msg.c_str(), msg.size());  // Send confirmation message back to the client
}

void load(int client_socket) {
    lock_guard<mutex> guard(myMutex);
    ifstream inFile("database.json");
    // Simplified JSON loading logic, should be replaced with robust parsing
    string jsonString((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    cout << "Database loaded: " << jsonString << endl;
    inFile.close();
    string msg = "Database loaded\n";
    write(client_socket, msg.c_str(), msg.size());  // Send confirmation message back to the client
}
