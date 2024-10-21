#include "Inode.h"
#include <fstream>
#include <iostream>
#include <ctime>

Inode::Inode(const std::string &disk, uint32_t inodeTableStart) : disk(disk), inodeTableStart(inodeTableStart) {}

void Inode::readInodeFromDisk(uint32_t inodeNumber) {
    std::ifstream diskFile(disk, std::ios::binary);
    if (!diskFile) {
        std::cerr << "Error opening disk file" << std::endl;
        return;
    }

    uint32_t inodeOffset = inodeTableStart + inodeNumber * sizeof(Ext4Inode);
    diskFile.seekg(inodeOffset, std::ios::beg);
    diskFile.read(reinterpret_cast<char *>(&inode), sizeof(inode));
    diskFile.close();
}

void Inode::writeInodeToDisk(uint32_t inodeNumber) {
    std::ofstream diskFile(disk, std::ios::binary | std::ios::in);
    if (!diskFile) {
        std::cerr << "Error opening disk file" << std::endl;
        return;
    }

    uint32_t inodeOffset = inodeTableStart + inodeNumber * sizeof(Ext4Inode);
    diskFile.seekp(inodeOffset, std::ios::beg);
    diskFile.write(reinterpret_cast<const char *>(&inode), sizeof(inode));
    diskFile.close();
}

void Inode::createInode(uint16_t mode, uint32_t size) {
    inode.i_mode = mode;
    inode.i_uid = 0;
    inode.i_size = size;
    inode.i_atime = inode.i_ctime = inode.i_mtime = 0;
    inode.i_dtime = 0;
    inode.i_gid = 0;
    inode.i_links_count = 1;
    inode.i_blocks = 0;
    inode.i_flags = 0;
    for (int i = 0; i < 15; ++i) {
        inode.i_block[i] = 0;
    }
}

void Inode::deleteInode() {
    inode.i_dtime = static_cast<uint32_t>(time(nullptr));
}
