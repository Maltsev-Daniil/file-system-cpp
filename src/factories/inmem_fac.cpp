//
// Created by nitsir on 12/24/25.
//

#include "../../include/factories/objects/inmem_fac.h"

#include "../../include/domain/objects/acl.h"
#include "../../include/domain/objects/inmem_file.h"

namespace domain {
std::unique_ptr<IInMemoryFile> InMemoryFactory::createInMemory(userId_t user_id, groupId_t group_id,
                                                               std::unique_ptr<IACL> acl)
{
    return std::make_unique<InMemoryFile>(inode_gen.generate(), user_id, group_id, 0, std::move(acl));
}

std::unique_ptr<IInMemoryFile> InMemoryFactory::createFullInMemory(inode_t id, userId_t user_id, groupId_t group_id, size_t size, std::unique_ptr<IACL> acl, TimePoint create_time, TimePoint mod_time) {
    return std::make_unique<InMemoryFile>(
        id,
        user_id,
        group_id,
        size,
        std::move(acl),
        create_time,
        mod_time);
}


void InMemoryFactory::generateId(IInMemoryFile *inmem)
{
    inmem->setId(inode_gen.generate());
}
}
