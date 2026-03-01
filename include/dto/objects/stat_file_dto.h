#pragma once

#include "domain/types.h"

namespace domain {
class IInMemoryFile;
}


struct StatFileDTO {    
    domain::TimePoint creation_time;
    domain::TimePoint modification_time;
    domain::userId_t uid;
    domain::groupId_t gid;
    size_t size;
    domain::inode_t id;

    StatFileDTO(domain::IInMemoryFile* inmem);
};