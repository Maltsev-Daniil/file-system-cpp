//
// Created by nitsir on 12/24/25.
//

#include "../../../include/domain/objects/inmem_file.h"
#include "../../../include/domain/objects/acl.h"

namespace domain {

InMemoryFile::InMemoryFile(inode_t id,
                           userId_t uid,
                           groupId_t gid,
                           size_t size,
                           std::unique_ptr<IACL> acl,
                           TimePoint creation_time,
                           TimePoint modification_time)
        : id(id)
        , creation_time(creation_time)
        , modification_time(modification_time)
        , uid(uid)
        , gid(gid)
        , acl(std::move(acl))
        , size(size)
{
}

InMemoryFile::~InMemoryFile() = default;

std::unique_ptr<IInMemoryFile> InMemoryFile::clone() const
{
    auto copy = std::make_unique<InMemoryFile>(
        this->id,
        this->uid,
        this->gid,
        this->size,
        this->acl->clone(),
        this->creation_time,
        this->modification_time
    );
    return copy;
}
}
