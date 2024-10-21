#ifndef INODE_H
#define INODE_H

#include <cstdint>
#include <string>

class Inode {
public:
    struct Ext4Inode {
        uint16_t i_mode;
        uint16_t i_uid;
        uint32_t i_size;
        uint32_t i_atime;
        uint32_t i_ctime;
        uint32_t i_mtime;
        uint32_t i_dtime;
        uint16_t i_gid;
        uint16_t i_links_count;
        uint32_t i_blocks;
        uint32_t i_flags;
        uint32_t i_block[15];
    };

    Inode(const std::string &disk, uint32_t inodeTableStart);
    void readInodeFromDisk(uint32_t inodeNumber);
    void writeInodeToDisk(uint32_t inodeNumber);
    void createInode(uint16_t mode, uint32_t size);
    void deleteInode();

    const Ext4Inode& getInode() const { return inode; }

private:
    std::string disk;
    uint32_t inodeTableStart;
    Ext4Inode inode;
};

#endif // INODE_H
