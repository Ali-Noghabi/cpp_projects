#include "BlockGroup.h"
#include <fstream>
#include <iostream>

BlockGroup::BlockGroup(const std::string &disk) : disk(disk) {}

void BlockGroup::readGroupDescFromDisk(uint32_t groupNumber) {
    std::ifstream diskFile(disk, std::ios::binary);
    if (!diskFile) {
        std::cerr << "Error opening disk file" << std::endl;
        return;
    }

    diskFile.seekg(groupNumber * sizeof(Ext4GroupDesc), std::ios::beg);
    diskFile.read(reinterpret_cast<char *>(&groupDesc), sizeof(groupDesc));
    diskFile.close();
}

void BlockGroup::writeGroupDescToDisk(uint32_t groupNumber) {
    std::ofstream diskFile(disk, std::ios::binary | std::ios::in);
    if (!diskFile) {
        std::cerr << "Error opening disk file" << std::endl;
        return;
    }

    diskFile.seekp(groupNumber * sizeof(Ext4GroupDesc), std::ios::beg);
    diskFile.write(reinterpret_cast<const char *>(&groupDesc), sizeof(groupDesc));
    diskFile.close();
}

int BlockGroup::findFreeInode(const std::vector<bool> &inodeBitmap) const {
    for (size_t i = 0; i < inodeBitmap.size(); ++i) {
        if (!inodeBitmap[i]) {
            return static_cast<int>(i);
        }
    }
    return -1; // No free inode
}

int BlockGroup::findFreeBlock(const std::vector<bool> &blockBitmap) const {
    for (size_t i = 0; i < blockBitmap.size(); ++i) {
        if (!blockBitmap[i]) {
            return static_cast<int>(i);
        }
    }
    return -1; // No free block
}

void BlockGroup::freeBlock(std::vector<bool> &blockBitmap, int blockIndex) {
    if (blockIndex >= 0 && blockIndex < static_cast<int>(blockBitmap.size())) {
        blockBitmap[blockIndex] = false;
    }
}
