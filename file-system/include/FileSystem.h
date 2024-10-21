#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "BlockGroup.h"
#include "Inode.h"
#include "Journal.h"
#include <string>

class FileSystem {
public:
    FileSystem(const std::string &disk);
    void initialize();
    void createFile(uint16_t mode, uint32_t size);
    void deleteFile(uint32_t inodeNumber);
    // Other file system operations...

private:
    BlockGroup blockGroup;
    Inode inode;
    Journal journal;
};

#endif // FILESYSTEM_H
