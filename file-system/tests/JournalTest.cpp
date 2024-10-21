#include "Journal.h"
#include <iostream>

int main() {
    std::string disk = "journal_disk.img";
    Journal journal(disk);

    // Write to the journal
    Journal::JournalEntry entry;
    entry.header.j_magic = Journal::MAGIC_NUMBER;
    entry.header.j_blocktype = 1;
    entry.header.j_sequence = 1;
    entry.data = {'H', 'e', 'l', 'l', 'o'};
    journal.writeJournal(entry);
    std::cout << "Journal entry written." << std::endl;

    // Read from the journal
    std::vector<Journal::JournalEntry> journalEntries;
    journal.readJournal(journalEntries);

    if (!journalEntries.empty()) {
        const auto& readEntry = journalEntries[0];
        std::cout << "Journal entry read:" << std::endl;
        std::cout << "Magic: " << readEntry.header.j_magic << std::endl;
        std::cout << "Blocktype: " << readEntry.header.j_blocktype << std::endl;
        std::cout << "Sequence: " << readEntry.header.j_sequence << std::endl;
        std::cout << "Data: " << std::string(readEntry.data.begin(), readEntry.data.end()) << std::endl;
    } else {
        std::cout << "No journal entries found." << std::endl;
    }

    return 0;
}
