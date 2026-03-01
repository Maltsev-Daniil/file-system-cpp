// //
// // Created by nitsir on 12/28/25.
// //
//
// #include "mappers/objects/file_object_mapper.h"
// #include "domain/objects/file_object.h"
//
//
// #include <stdexcept>
//
// #include "domain/objects/directory.h"
// #include "domain/objects/reg_file.h"
// #include "snapshots/objects/file_object_snapshot.h"
// #include "snapshots/objects/regular_file_snapshot.h"
// #include "snapshots/objects/directory_snapshot.h"
//
// std::unique_ptr<FileObjectSnapshot>
// FileObjectMapper::toSnapshot(const domain::IFileObject& object) const
// {
//     if (const auto* file =
//             dynamic_cast<const domain::IRegularFile*>(&object))
//     {
//         return file_mapper.toSnapshot(*file);
//     }
//
//     if (const auto* dir =
//             dynamic_cast<const domain::IDirectory*>(&object))
//     {
//         return dir_mapper->toSnapshot(*dir);
//     }
//
//     throw std::logic_error(
//         "FileObjectMapper::toSnapshot: unknown IFileObject implementation"
//     );
// }
//
// std::unique_ptr<domain::IFileObject>
// FileObjectMapper::fromSnapshot(const FileObjectSnapshot& snapshot) const
// {
//     if (const auto* file =
//             dynamic_cast<const RegularFileSnapshot*>(&snapshot))
//     {
//         return file_mapper.fromSnapshot(*file);
//     }
//
//     if (const auto* dir =
//             dynamic_cast<const DirectorySnapshot*>(&snapshot))
//     {
//         return dir_mapper->fromSnapshot(*dir);
//     }
//
//     throw std::logic_error(
//         "FileObjectMapper::fromSnapshot: unknown FileObjectSnapshot type"
//     );
// }

#include "mappers/objects/file_object_mapper.h"
#include "domain/objects/file_object.h"

#include <stdexcept>
#include "snapshots/objects/file_object_snapshot.h"

#include "mappers/objects/adapters/ifo_adapter_mapper.h"

void PolymorphicFileObjectMapper::registerAdapter(
    std::unique_ptr<IFileObjectMapperAdapter> adapter) {
    if (!adapter) return;

    auto entity_type = adapter->getEntityType();
    auto snapshot_type = adapter->getSnapshotType();

    by_entity_[entity_type] = adapter.get();
    by_snapshot_[snapshot_type] = adapter.get();

    adapters_.push_back(std::move(adapter));
}

std::unique_ptr<FileObjectSnapshot>
PolymorphicFileObjectMapper::toSnapshot(const domain::IFileObject& object) const {
    auto it = by_entity_.find(typeid(object));
    if (it != by_entity_.end()) {
        return it->second->toSnapshot(object);
    }
    throw std::logic_error("No adapter registered for IFileObject type");
}

std::unique_ptr<domain::IFileObject>
PolymorphicFileObjectMapper::fromSnapshot(const FileObjectSnapshot& snapshot) const {
    auto it = by_snapshot_.find(typeid(snapshot));
    if (it != by_snapshot_.end()) {
        return it->second->fromSnapshot(snapshot);
    }
    throw std::logic_error("No adapter registered for snapshot type");
}
