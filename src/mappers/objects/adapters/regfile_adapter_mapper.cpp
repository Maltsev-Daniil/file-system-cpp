//
// Created by nitsir on 1/14/26.
//

#include "../../../../include/mappers/objects/adapters/regfile_adapter_mapper.h"

#include "domain/objects/reg_file.h"
#include "mappers/objects/regfile_mapper.h"
#include "snapshots/objects/regular_file_snapshot.h"


RegularFileMapperAdapter::RegularFileMapperAdapter(IRegularFileMapper& mapper)
    : mapper_(mapper) {}

std::type_index RegularFileMapperAdapter::getEntityType() const {
    return typeid(domain::IRegularFile);
}

std::type_index RegularFileMapperAdapter::getSnapshotType() const {
    return typeid(RegularFileSnapshot);
}

std::unique_ptr<FileObjectSnapshot>
RegularFileMapperAdapter::toSnapshot(const domain::IFileObject& entity) const {
    return mapper_.toSnapshot(static_cast<const domain::IRegularFile&>(entity));
}

std::unique_ptr<domain::IFileObject>
RegularFileMapperAdapter::fromSnapshot(const FileObjectSnapshot& snapshot) const {
    return mapper_.fromSnapshot(static_cast<const RegularFileSnapshot&>(snapshot));
}