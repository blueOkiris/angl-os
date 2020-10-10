#pragma once

#define DIR_ENTRY_SIZE      16
#define FILENAME_SIZE       8
#define FILETYPE_SIZE       1
#define BLOCK_SIZE          1024

#define FILE_SYSTEM_START   0x84F000

namespace angl {
    namespace io {
        struct DirEntry {
            char *fileName;
            uint32_t *fileType;
            uint32_t *inode;
        };
        
        struct Inode {
            uint32_t fileSize;
            uint32_t *dataBlock;
        };
        
        class FileSystem {
            private:
                uint32_t _numDirEntries;
                uint32_t _numInodes;
                uint32_t _numBlocks;
                uint32_t *_fileSystem;
            
            public:
                void init();
                const DirEntry readDirEntryByName(const char *fileName);
                const DirEntry readDirEntryByIndex(const uint32_t &index);
                bool tryReadData(
                    const uint32_t &inode, const uint32_t &offset,
                    uint8_t *buffer, const uint32_t &len
                );
        };
    }
}
