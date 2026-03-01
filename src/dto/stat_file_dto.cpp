#include "domain/objects/inmem_file.h"
#include "../../include/dto/objects/stat_file_dto.h"
#include <stdexcept>

StatFileDTO::StatFileDTO(domain::IInMemoryFile *inmem)
{
    if (!inmem)
        throw std::invalid_argument("statFileDTO: null pointer given");
    creation_time = inmem->getCreateTime();
    modification_time = inmem->getModTime();
    uid = inmem->getUid();
    gid = inmem->getGid();
    id = inmem->getId();
    size = inmem->getSize();
}