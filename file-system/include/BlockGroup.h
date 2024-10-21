#ifndef BLOCKGROUP_H
#define BLOCKGROUP_H

#include <cstdint>
#include <string>
#include <vector>

class BlockGroup {
public:
    struct Ext4GroupDesc {
        uint32_t bg_block_bitmap;
        uint32_t bg_inode_bitmap;
        uint32_t bg_inode_table;
        uint16_t bg_free_blocks_count;
        uint16_t bg_free_inodes_count;
        uint16_t bg_used_dirs_count;
        uint16_t bg_flags;
        uint32_t bg_reserved[3];
    };

    BlockGroup(const std::string &disk);

    void readGroupDescFromDisk(uint32_t groupNumber);
    void writeGroupDescToDisk(uint32_t groupNumber);
    int findFreeInode(const std::vector<bool> &inodeBitmap) const;
    int findFreeBlock(const std::vector<bool> &blockBitmap) const;
    void freeBlock(std::vector<bool> &blockBitmap, int blockIndex);

    const std::string& getDisk() const { return disk; }
    Ext4GroupDesc& getGroupDesc() { return groupDesc; }

private:
    std::string disk;
    Ext4GroupDesc groupDesc;
};

#endif // BLOCKGROUP_H
