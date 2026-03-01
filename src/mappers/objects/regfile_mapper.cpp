#include "mappers/objects/regfile_mapper.h"

#include "domain/objects/reg_file.h"
#include "domain/objects/inmem_file.h"
#include "mappers/objects/metadata_mapper.h"
#include "snapshots/objects/file_metadata_snapshot.h"
#include "snapshots/objects/regular_file_snapshot.h"
//
// Created by nitsir on 12/28/25.
//
std::unique_ptr<RegularFileSnapshot>
RegularFileMapper::toSnapshot(const domain::IRegularFile& file) const
{
    const auto* in_memory = file.getInMemoryFile();

    FileMetadataSnapshot metadata_snapshot =
        metadata_mapper_.toSnapshot(*in_memory);

    return std::make_unique<RegularFileSnapshot>(
        file.getName(),
        file.getPath(),
        std::move(metadata_snapshot),
        file.getContent()
    );
}

std::unique_ptr<domain::IRegularFile>
RegularFileMapper::fromSnapshot(const RegularFileSnapshot &snapshot) const {
    auto inmem =
        metadata_mapper_.fromSnapshot(snapshot.getMetadata());
    return std::make_unique<domain::RegularFile>(
        std::move(inmem),
        snapshot.getName(),
        snapshot.getPath(),
        domain::LockType::NONE,
        snapshot.getContent()
    );
}