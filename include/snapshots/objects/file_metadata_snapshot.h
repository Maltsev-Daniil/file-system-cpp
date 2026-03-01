#pragma once

#include <cstdint>
#include <string>

#include "domain/objects/acl.h"
#include "snapshots/acl/acl_item_snapshot.h"
#include "snapshots/acl/acl_snapshot.h"

class FileMetadataSnapshot {
public:
    FileMetadataSnapshot(
        std::size_t id,
        std::uint64_t created_at,
        std::uint64_t modified_at,
        std::uint64_t owner_user_id,
        std::uint64_t owner_group_id,
        std::uint64_t size,
        std::unique_ptr<AclSnapshot> acl
    ) : id(id),
     created_at(created_at),
     modified_at(modified_at),
     owner_user_id(owner_user_id),
     owner_group_id(owner_group_id),
     size(size),
     acl(std::move(acl)) {}


    std::uint64_t getCreatedAt() const noexcept { return created_at; };
    std::uint64_t getModifiedAt() const noexcept { return modified_at; };
    std::uint64_t getOwnerUserId() const noexcept { return owner_user_id; };
    std::uint64_t getOwnerGroupId() const noexcept { return owner_group_id; };
    std::uint64_t getSize() const noexcept { return size; };
    const AclSnapshot* getAcl() const noexcept { return acl.get(); };
    std::size_t getId() const noexcept { return id; };

private:
    std::size_t id;
    std::uint64_t created_at;
    std::uint64_t modified_at;
    std::uint64_t owner_user_id;
    std::uint64_t owner_group_id;
    std::uint64_t size;
    std::unique_ptr<AclSnapshot> acl;
};
