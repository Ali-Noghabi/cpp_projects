#include "Journal.h"
#include <fstream>
#include <iostream>

Journal::Journal(const std::string &disk) : disk(disk) {}

void Journal::writeJournal(const JournalEntry &entry) {
    std::ofstream diskFile(disk, std::ios::binary | std::ios::app);
    if (!diskFile) {
        std::cerr << "Error opening disk file" << std::endl;
        return;
    }

    diskFile.write(reinterpret_cast<const char *>(&entry.header), sizeof(entry.header));
    uint32_t dataSize = entry.data.size();
    diskFile.write(reinterpret_cast<const char *>(&dataSize), sizeof(dataSize));
    diskFile.write(entry.data.data(), dataSize);
    diskFile.close();
}

void Journal::readJournal(std::vector<JournalEntry> &journalEntries) {
    std::ifstream diskFile(disk, std::ios::binary);
    if (!diskFile) {
        std::cerr << "Error opening disk file" << std::endl;
        return;
    }

    JournalEntry entry;
    while (diskFile.read(reinterpret_cast<char *>(&entry.header), sizeof(entry.header))) {
        uint32_t dataSize;
        diskFile.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
        entry.data.resize(dataSize);
        diskFile.read(entry.data.data(), dataSize);
        journalEntries.push_back(entry);
    }
    diskFile.close();
}

void Journal::manageJournal(std::vector<JournalEntry> &journalEntries, size_t maxEntries) {
    if (journalEntries.size() > maxEntries) {
        journalEntries.erase(journalEntries.begin(), journalEntries.begin() + (journalEntries.size() - maxEntries));
    }
}
