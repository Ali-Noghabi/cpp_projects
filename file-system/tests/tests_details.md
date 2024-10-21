#### Tests Explanation

This file contains tests for the `BlockGroup`, `Inode`, and `Journal` classes. Below is a detailed explanation of each test case and the expected output.

---

### BlockGroup Tests

#### `BlockGroupTest.FindFreeInode`
- **Description**: Tests the function that finds a free inode in the inode bitmap.
- **Expected Output**:
  - When the bitmap is `{false, true, true, false}`, the first free inode should be at index `0`.
  - When the bitmap is updated to `{true, true, true, false}`, the first free inode should be at index `3`.
  - When the bitmap is `{true, true, true, true}`, there should be no free inodes (`-1`).

#### `BlockGroupTest.FindFreeBlock`
- **Description**: Tests the function that finds a free block in the block bitmap.
- **Expected Output**:
  - When the bitmap is `{false, true, true, false}`, the first free block should be at index `0`.
  - When the bitmap is updated to `{true, true, true, false}`, the first free block should be at index `3`.
  - When the bitmap is `{true, true, true, true}`, there should be no free blocks (`-1`).

#### `BlockGroupTest.FreeBlock`
- **Description**: Tests the function that frees a block in the block bitmap.
- **Expected Output**:
  - Given a block bitmap `{true, true, true, true}`, freeing block at index `1` should result in `{true, false, true, true}`.

#### `BlockGroupTest.ReadWriteGroupDesc`
- **Description**: Tests reading and writing the group descriptor to and from the disk.
- **Expected Output**:
  - After writing a group descriptor to the disk and reading it back, the values should match the original descriptor.

---

### Inode Tests

#### `InodeTest.CreateInode`
- **Description**: Tests the creation of an inode.
- **Expected Output**:
  - After creating an inode with mode `0x1FF` and size `1024`, the inode's attributes should match these values.

#### `InodeTest.ReadWriteInode`
- **Description**: Tests reading and writing an inode to the disk.
- **Expected Output**:
  - After writing an inode to the disk and reading it back, the inode's attributes should match the original values.

#### `InodeTest.DeleteInode`


- **Description**: Tests the deletion of an inode.
- **Expected Output**:
  - After deleting an inode and writing it to the disk, the deletion time (`i_dtime`) should be set (not `0`).

---

### Journal Tests

#### `JournalTest.WriteReadJournal`
- **Description**: Tests writing and reading a journal entry.
- **Expected Output**:
  - After writing a journal entry with specific header values and data `{'H', 'e', 'l', 'l', 'o'}`, reading it back should return the same values.

#### `JournalTest.ManageJournal`
- **Description**: Tests managing the journal size by limiting the number of entries.
- **Expected Output**:
  - After managing a journal with a maximum size of `2`, the oldest entry should be removed, and the remaining entries should have the correct sequence numbers.
