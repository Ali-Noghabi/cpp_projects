#include "BlockGroup.h"
#include "Inode.h"
#include "Journal.h"
#include <gtest/gtest.h>
#include <fstream>

// Helper function to initialize a disk image with zeros
void initializeDisk(const std::string& disk) {
    std::ofstream diskFile(disk, std::ios::binary);
    if (!diskFile) {
        std::cerr << "Error creating disk file" << std::endl;
        return;
    }

    // Initialize disk with zeros or some predefined data
    std::vector<char> emptyData(1024 * 1024, 0); // 1MB disk for simplicity
    diskFile.write(emptyData.data(), emptyData.size());
    diskFile.close();
}

// Test case for finding a free inode in the bitmap
TEST(BlockGroupTest, FindFreeInode) {
    std::vector<bool> inodeBitmap = {false, true, true, false};
    BlockGroup bg("disk.img");
    EXPECT_EQ(bg.findFreeInode(inodeBitmap), 0);
    inodeBitmap[0] = true;
    EXPECT_EQ(bg.findFreeInode(inodeBitmap), 3);
    inodeBitmap[3] = true;
    EXPECT_EQ(bg.findFreeInode(inodeBitmap), -1);
}

// Test case for finding a free block in the bitmap
TEST(BlockGroupTest, FindFreeBlock) {
    std::vector<bool> blockBitmap = {false, true, true, false};
    BlockGroup bg("disk.img");
    EXPECT_EQ(bg.findFreeBlock(blockBitmap), 0);
    blockBitmap[0] = true;
    EXPECT_EQ(bg.findFreeBlock(blockBitmap), 3);
    blockBitmap[3] = true;
    EXPECT_EQ(bg.findFreeBlock(blockBitmap), -1);
}

// Test case for freeing a block in the bitmap
TEST(BlockGroupTest, FreeBlock) {
    std::vector<bool> blockBitmap = {true, true, true, true};
    BlockGroup bg("disk.img");
    bg.freeBlock(blockBitmap, 1);
    EXPECT_FALSE(blockBitmap[1]);
}

// Test case for reading and writing group descriptor to disk
TEST(BlockGroupTest, ReadWriteGroupDesc) {
    initializeDisk("disk.img");
    BlockGroup bg("disk.img");
    BlockGroup::Ext4GroupDesc desc = {1, 2, 3, 4, 5, 6, 7, {0, 0, 0}};
    bg.getGroupDesc() = desc;
    bg.writeGroupDescToDisk(0);

    BlockGroup bg2("disk.img");
    bg2.readGroupDescFromDisk(0);
    BlockGroup::Ext4GroupDesc desc2 = bg2.getGroupDesc();

    EXPECT_EQ(desc.bg_block_bitmap, desc2.bg_block_bitmap);
    EXPECT_EQ(desc.bg_inode_bitmap, desc2.bg_inode_bitmap);
    EXPECT_EQ(desc.bg_inode_table, desc2.bg_inode_table);
    EXPECT_EQ(desc.bg_free_blocks_count, desc2.bg_free_blocks_count);
    EXPECT_EQ(desc.bg_free_inodes_count, desc2.bg_free_inodes_count);
    EXPECT_EQ(desc.bg_used_dirs_count, desc2.bg_used_dirs_count);
    EXPECT_EQ(desc.bg_flags, desc2.bg_flags);
}

// Test case for creating an inode
TEST(InodeTest, CreateInode) {
    initializeDisk("disk.img");
    Inode inode("disk.img", 3);
    inode.createInode(0x1FF, 1024);

    const auto& ext4Inode = inode.getInode();
    EXPECT_EQ(ext4Inode.i_mode, 0x1FF);
    EXPECT_EQ(ext4Inode.i_size, 1024);
    EXPECT_EQ(ext4Inode.i_links_count, 1);
}

// Test case for reading and writing an inode to disk
TEST(InodeTest, ReadWriteInode) {
    initializeDisk("disk.img");
    Inode inode("disk.img", 3);
    inode.createInode(0x1FF, 1024);
    inode.writeInodeToDisk(0);

    Inode inode2("disk.img", 3);
    inode2.readInodeFromDisk(0);

    const auto& ext4Inode2 = inode2.getInode();
    EXPECT_EQ(ext4Inode2.i_mode, 0x1FF);
    EXPECT_EQ(ext4Inode2.i_size, 1024);
    EXPECT_EQ(ext4Inode2.i_links_count, 1);
}

// Test case for deleting an inode
TEST(InodeTest, DeleteInode) {
    initializeDisk("disk.img");
    Inode inode("disk.img", 3);
    inode.createInode(0x1FF, 1024);
    inode.writeInodeToDisk(0);

    inode.deleteInode();
    inode.writeInodeToDisk(0);

    Inode inode2("disk.img", 3);
    inode2.readInodeFromDisk(0);

    const auto& ext4Inode2 = inode2.getInode();
    EXPECT_NE(ext4Inode2.i_dtime, 0);
}

// Test case for writing and reading a journal entry
TEST(JournalTest, WriteReadJournal) {
    std::string disk = "journal_disk.img";
    Journal journal(disk);

    // Write to the journal
    Journal::JournalEntry entry;
    entry.header.j_magic = Journal::MAGIC_NUMBER;
    entry.header.j_blocktype = 1;
    entry.header.j_sequence = 1;
    entry.data = {'H', 'e', 'l', 'l', 'o'};
    journal.writeJournal(entry);

    // Read from the journal
    std::vector<Journal::JournalEntry> journalEntries;
    journal.readJournal(journalEntries);

    ASSERT_FALSE(journalEntries.empty());
    EXPECT_EQ(journalEntries[0].header.j_magic, Journal::MAGIC_NUMBER);
    EXPECT_EQ(journalEntries[0].header.j_blocktype, 1);
    EXPECT_EQ(journalEntries[0].header.j_sequence, 1);
    std::vector<char> expectedData = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(journalEntries[0].data, expectedData);
}

// Test case for managing the journal size
TEST(JournalTest, ManageJournal) {
    std::vector<Journal::JournalEntry> journalEntries = {
        {{Journal::MAGIC_NUMBER, 1, 1}, {'D', 'a', 't', 'a', '1'}},
        {{Journal::MAGIC_NUMBER, 1, 2}, {'D', 'a', 't', 'a', '2'}},
        {{Journal::MAGIC_NUMBER, 1, 3}, {'D', 'a', 't', 'a', '3'}}
    };

    Journal journal("journal_disk.img");
    journal.manageJournal(journalEntries, 2);
    ASSERT_EQ(journalEntries.size(), 2);
    EXPECT_EQ(journalEntries[0].header.j_sequence, 2);
    EXPECT_EQ(journalEntries[1].header.j_sequence, 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
