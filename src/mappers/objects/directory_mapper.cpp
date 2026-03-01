//
// Created by nitsir on 12/28/25.
//

#include <algorithm>

#include "domain/objects/directory_structure.h"
#include "domain/objects/directory.h"
#include "domain/objects/inmem_file.h"
#include "mappers/objects/directory_mapper.h"

#include "mappers/objects/file_object_mapper.h"
#include "mappers/objects/metadata_mapper.h"
#include "snapshots/objects/directory_snapshot.h"

std::unique_ptr<DirectorySnapshot>
DirectoryMapper::toSnapshot(const domain::IDirectory& directory) const
{
    const auto* in_memory = directory.getInMemoryFile();
    if (!in_memory) {
        throw std::logic_error(
            "DirectoryMapper::toSnapshot: directory has no in-memory file"
        );
    }

    FileMetadataSnapshot metadata_snapshot =
        metadata_mapper_.toSnapshot(*in_memory);

    std::vector<std::unique_ptr<FileObjectSnapshot>> children_snapshots;
    children_snapshots.reserve(directory.getDirectoryStructure()->size());

    for (const auto* child :
            directory.getDirectoryStructure()->getObjs()) {
        children_snapshots.push_back(
            file_object_mapper_.toSnapshot(*child)
        );
    }

    return std::make_unique<DirectorySnapshot>(
        directory.getName(),
        directory.getPath(),
        std::move(metadata_snapshot),
        std::move(children_snapshots)
    );
}

std::unique_ptr<domain::IDirectory>
DirectoryMapper::fromSnapshot(const DirectorySnapshot& snapshot) const
{
    auto in_memory =
        metadata_mapper_.fromSnapshot(snapshot.getMetadata());

    auto dir_structure =
        std::make_unique<domain::DirectoryStructure>();

    auto directory = std::make_unique<domain::Directory>(
        std::move(in_memory),
        std::move(dir_structure),
        snapshot.getName(),
        snapshot.getPath()
    );

    for (const auto& child_snapshot : snapshot.getChildren()) {
        auto child =
            file_object_mapper_.fromSnapshot(*child_snapshot);

        directory->getDirectoryStructure()->addEntry(std::move(child));
    }

    return directory;
}