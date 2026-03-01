//
// Created by nitsir on 1/14/26.
//

#include "../../../../include/mappers/objects/adapters/directory_adapter_mapper.h"

#include "domain/objects/directory.h"
#include "mappers/objects/directory_mapper.h"
#include "snapshots/objects/directory_snapshot.h"

DirectoryMapperAdapter::DirectoryMapperAdapter(IDirectoryMapper& mapper)
    : mapper_(mapper) {}

std::type_index DirectoryMapperAdapter::getEntityType() const {
    return typeid(domain::IDirectory);
}

std::type_index DirectoryMapperAdapter::getSnapshotType() const {
    return typeid(DirectorySnapshot);
}

std::unique_ptr<FileObjectSnapshot>
DirectoryMapperAdapter::toSnapshot(const domain::IFileObject& entity) const {
    return mapper_.toSnapshot(static_cast<const domain::IDirectory&>(entity));
}

std::unique_ptr<domain::IFileObject>
DirectoryMapperAdapter::fromSnapshot(const FileObjectSnapshot& snapshot) const {
    return mapper_.fromSnapshot(static_cast<const DirectorySnapshot&>(snapshot));
}