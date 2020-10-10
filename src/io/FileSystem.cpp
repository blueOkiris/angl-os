#include <stddef.h>
#include <stdint.h>
#include <io/FileSystem.hpp>

using namespace angl;
using namespace io;

void FileSystem::init() {
    _fileSystem = (uint32_t *) FILE_SYSTEM_START;
    
    _numDirEntries = *_fileSystem;
    _numInodes = *(_fileSystem + 1);
    _numBlocks = *(_fileSystem + 2);
}

inline int strlen(const char *str) {
    int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

inline bool strequ(const char *str1, const char *str2) {
    if(strlen(str1) != strlen(str2)) {
        return false;
    }
    
    for(int i = 0; i < strlen(str1); i++) {
        if(str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

const DirEntry FileSystem::readDirEntryByName(const char *fileName) {
    for(uint8_t i = 0; i < _numDirEntries; i++) {
        auto dirEntryFileName = reinterpret_cast<char *>(
            _fileSystem + (i + 1) * DIR_ENTRY_SIZE
        );
        
        if(strequ(fileName, 
                    reinterpret_cast<const char*>(dirEntryFileName)
                )) {
            return {
                dirEntryFileName,
                reinterpret_cast<uint32_t *>(dirEntryFileName) + FILENAME_SIZE,
                reinterpret_cast<uint32_t *>(dirEntryFileName)
                    + FILENAME_SIZE + FILETYPE_SIZE
            };
        }
    }
        
    return { NULL, NULL, NULL };
}

const DirEntry FileSystem::readDirEntryByIndex(const uint32_t &index) {
    if(index >= _numDirEntries || index < 0) {
        return { NULL, NULL, NULL };
    }
    
    auto targetDirEntry = reinterpret_cast<char *>(
        _fileSystem + DIR_ENTRY_SIZE * (index + 1)
    );
    
    return {
        targetDirEntry,
        reinterpret_cast<uint32_t *>(targetDirEntry) + FILENAME_SIZE,
        reinterpret_cast<uint32_t *>(targetDirEntry)
            + FILENAME_SIZE + FILETYPE_SIZE
    };
}

bool FileSystem::tryReadData(
        const uint32_t &inode, const uint32_t &offset,
        uint8_t *buffer, const uint32_t &len) {
    if(inode > _numInodes) {
        return false;
    }
    
    auto targetInode = reinterpret_cast<Inode *>(
        _fileSystem + (inode + 1) * BLOCK_SIZE
    );
    
    auto bytesRead = 0;
    auto bytesRemaining = 0;
    if(targetInode->fileSize == 0) {
        return false;
    } else if(len > targetInode->fileSize - offset) {
        bytesRemaining = targetInode->fileSize - offset;
    } else {
        bytesRemaining = len;
    }
    
    auto blockIndex = 0;
    while(bytesRemaining > 0) {
        auto blockNum = *(targetInode->dataBlock + blockIndex);
        
        // Invalid block number - bad inode
        if(blockNum > _numBlocks || blockNum < 0) {
            return false;
        }
        
        auto blockPtr = reinterpret_cast<uint8_t *>(
            _fileSystem + (blockNum + _numInodes + 1) * BLOCK_SIZE
        );
        for(int i = 0; i < BLOCK_SIZE; i++) {
            buffer[i] = blockPtr[i];
        }
        
        bytesRead += 4 * BLOCK_SIZE;
        bytesRemaining -= 4 * BLOCK_SIZE;
        blockIndex++;
    }
    
    return true;
}