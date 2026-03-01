#include "mappers/objects/metadata_mapper.h"

#include "domain/objects/inmem_file.h"
#include "mappers/acl/acl_mapper.h"

#include <chrono>

#include "factories/objects/inmem_fac.h"
#include "snapshots/objects/file_metadata_snapshot.h"

namespace {

std::uint64_t toUnixTs(const domain::TimePoint& tp)
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        tp.time_since_epoch()
    ).count();
}

domain::TimePoint fromUnixTs(std::uint64_t ts)
{
    return domain::TimePoint{ std::chrono::seconds{ts} };
}

}

FileMetadataSnapshot
FileMetadataMapper::toSnapshot(const domain::IInMemoryFile& metadata) const
{
    auto acl_snapshot =
        std::make_unique<AclSnapshot>(
            acl_mapper_.toSnapshot(*metadata.getAcl())
        );

    return FileMetadataSnapshot(
        metadata.getId(),
        toUnixTs(metadata.getCreateTime()),
        toUnixTs(metadata.getModTime()),
        metadata.getUid(),
        metadata.getGid(),
        metadata.getSize(),
        std::move(acl_snapshot)
    );
}

std::unique_ptr<domain::IInMemoryFile>
FileMetadataMapper::fromSnapshot(const FileMetadataSnapshot& snapshot) const
{
    auto acl =
        acl_mapper_.fromSnapshot(*snapshot.getAcl());

    return in_memory_factory_.createFullInMemory(
        snapshot.getId(),
        snapshot.getOwnerUserId(),
        snapshot.getOwnerGroupId(),
        snapshot.getSize(),
        std::move(acl),
        fromUnixTs(snapshot.getCreatedAt()),
        fromUnixTs(snapshot.getModifiedAt())
    );
}