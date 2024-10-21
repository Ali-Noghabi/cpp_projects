#include "FileSystem.h"
#include <fstream>
#include <iostream>
#include <vector>

FileSystem::FileSystem(const std::string &disk)
    : blockGroup(disk), inode(disk, 0), journal(disk) {}

void FileSystem::initialize() {
    // For simplicity, we'll initialize a dummy disk image with a simple layout
    std::ofstream diskFile(blockGroup.getDisk(), std::ios::binary | std::ios::trunc);
    if (!diskFile) {
        std::cerr << "Error opening disk file for initialization" << std::endl;
        return;
    }

    // Initialize with zeroed-out data
    char zero = 0;
    for (int i = 0; i < 1024 * 1024; ++i) { // 1MB of disk space
        diskFile.write(&zero, sizeof(zero));
    }

    // Initialize a block group descriptor
    BlockGroup::Ext4GroupDesc bgDesc = {};
    bgDesc.bg_block_bitmap = 1; // Let's assume block bitmap starts at block 1
    bgDesc.bg_inode_bitmap = 2; // Inode bitmap starts at block 2
    bgDesc.bg_inode_table = 3; // Inode table starts at block 3

    blockGroup.getGroupDesc() = bgDesc;
    blockGroup.writeGroupDescToDisk(0);

    // Initialize the bitmaps
    std::vector<bool> blockBitmap(1024, false); // Assuming 1024 blocks
    std::vector<bool> inodeBitmap(256, false); // Assuming 256 inodes

    std::ofstream blockBitmapFile(blockGroup.getDisk(), std::ios::binary | std::ios::in | std::ios::out);
    if (blockBitmapFile) {
        blockBitmapFile.seekp(1024, std::ios::beg); // Block 1
        for (auto bit : blockBitmap) {
            blockBitmapFile.write(reinterpret_cast<char*>(&bit), sizeof(bit));
        }

        blockBitmapFile.seekp(2048, std::ios::beg); // Block 2
        for (auto bit : inodeBitmap) {
            blockBitmapFile.write(reinterpret_cast<char*>(&bit), sizeof(bit));
        }
    }

    std::cout << "File system initialized." << std::endl;
}

void FileSystem::createFile(uint16_t mode, uint32_t size) {
    // Find a free inode
    std::vector<bool> inodeBitmap(256, false); // Assuming 256 inodes
    blockGroup.readGroupDescFromDisk(0);
    
    std::ifstream inodeBitmapFile(blockGroup.getDisk(), std::ios::binary);
    if (inodeBitmapFile) {
        inodeBitmapFile.seekg(2048, std::ios::beg); // Block 2
        for (auto &bit : inodeBitmap) {
            inodeBitmapFile.read(reinterpret_cast<char*>(&bit), sizeof(bit));
        }
    }

    int freeInodeIndex = blockGroup.findFreeInode(inodeBitmap);
    if (freeInodeIndex == -1) {
        std::cerr << "No free inodes available" << std::endl;
        return;
    }

    inodeBitmap[freeInodeIndex] = true;
    std::ofstream inodeBitmapOutFile(blockGroup.getDisk(), std::ios::binary | std::ios::in | std::ios::out);
    if (inodeBitmapOutFile) {
        inodeBitmapOutFile.seekp(2048, std::ios::beg); // Block 2
        for (auto bit : inodeBitmap) {
            inodeBitmapOutFile.write(reinterpret_cast<char*>(&bit), sizeof(bit));
        }
    }

    // Create the inode
    inode.createInode(mode, size);
    inode.writeInodeToDisk(freeInodeIndex);

    std::cout << "File created with inode number: " << freeInodeIndex << std::endl;
}

void FileSystem::deleteFile(uint32_t inodeNumber) {
    // Read the inode bitmap
    std::vector<bool> inodeBitmap(256, false); // Assuming 256 inodes

    std::ifstream inodeBitmapFile(blockGroup.getDisk(), std::ios::binary);
    if (inodeBitmapFile) {
        inodeBitmapFile.seekg(2048, std::ios::beg); // Block 2
        for (auto &bit : inodeBitmap) {
            inodeBitmapFile.read(reinterpret_cast<char*>(&bit), sizeof(bit));
        }
    }

    if (inodeNumber >= inodeBitmap.size() || !inodeBitmap[inodeNumber]) {
        std::cerr << "Invalid inode number or inode not in use" << std::endl;
        return;
    }

    // Mark the inode as free
    inodeBitmap[inodeNumber] = false;

    std::ofstream inodeBitmapOutFile(blockGroup.getDisk(), std::ios::binary | std::ios::in | std::ios::out);
    if (inodeBitmapOutFile) {
        inodeBitmapOutFile.seekp(2048, std::ios::beg); // Block 2
        for (auto bit : inodeBitmap) {
            inodeBitmapOutFile.write(reinterpret_cast<char*>(&bit), sizeof(bit));
        }
    }

    // Read and delete the inode
    inode.readInodeFromDisk(inodeNumber);
    inode.deleteInode();
    inode.writeInodeToDisk(inodeNumber);

    std::cout << "File with inode number " << inodeNumber << " deleted." << std::endl;
}
