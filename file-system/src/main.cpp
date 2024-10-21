#include "FileSystem.h"
#include <iostream>
#include <fstream>

void writeToFile(const std::string& disk, uint32_t inodeNumber, const std::string& content) {
    // Example implementation of writing content to a file's data block
    std::ofstream diskFile(disk, std::ios::binary | std::ios::in | std::ios::out);
    if (!diskFile) {
        std::cerr << "Error opening disk file for writing" << std::endl;
        return;
    }

    uint32_t blockStart = 4096; // Starting point for data blocks in this example
    diskFile.seekp(blockStart + inodeNumber * 1024, std::ios::beg); // Simple calculation for example purposes
    diskFile.write(content.c_str(), content.size());
    diskFile.close();
}

std::string readFromFile(const std::string& disk, uint32_t inodeNumber) {
    // Example implementation of reading content from a file's data block
    std::ifstream diskFile(disk, std::ios::binary);
    if (!diskFile) {
        std::cerr << "Error opening disk file for reading" << std::endl;
        return "";
    }

    uint32_t blockStart = 4096; // Starting point for data blocks in this example
    diskFile.seekg(blockStart + inodeNumber * 1024, std::ios::beg); // Simple calculation for example purposes
    char buffer[1024] = {0};
    diskFile.read(buffer, sizeof(buffer));
    diskFile.close();

    return std::string(buffer);
}

int main() {
    FileSystem fs("disk.img");
    fs.initialize();

    fs.createFile(0x1FF, 1024); // Create a file
    uint32_t inodeNumber = 0; // Assuming inode 0 for this example

    // Write to the file
    writeToFile("disk.img", inodeNumber, "Hello, this is a test file!");

    // Read from the file
    std::string content = readFromFile("disk.img", inodeNumber);
    std::cout << "Read from file: " << content << std::endl;

    // Delete the file
    fs.deleteFile(inodeNumber);

    return 0;
}
