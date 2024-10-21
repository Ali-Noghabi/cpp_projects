#ifndef JOURNAL_H
#define JOURNAL_H

#include <cstdint>
#include <string>
#include <vector>

class Journal {
public:
    struct Ext4JournalHeader {
        uint32_t j_magic;
        uint32_t j_blocktype;
        uint32_t j_sequence;
    };

    struct JournalEntry {
        Ext4JournalHeader header;
        std::vector<char> data;
    };

    Journal(const std::string &disk);
    void writeJournal(const JournalEntry &entry);
    void readJournal(std::vector<JournalEntry> &journalEntries);
    void manageJournal(std::vector<JournalEntry> &journalEntries, size_t maxEntries);

    static const uint32_t MAGIC_NUMBER = 0xC03B3998;
private:
    std::string disk;
};

#endif // JOURNAL_H
