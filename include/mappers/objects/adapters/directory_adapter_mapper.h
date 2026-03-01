#pragma once
#include "ifo_adapter_mapper.h"

class IDirectoryMapper;

class DirectoryMapperAdapter final : public IFileObjectMapperAdapter {
    IDirectoryMapper& mapper_;

public:
    explicit DirectoryMapperAdapter(IDirectoryMapper& mapper);

    std::type_index getEntityType() const override;

    std::type_index getSnapshotType() const override;

    std::unique_ptr<FileObjectSnapshot>
    toSnapshot(const domain::IFileObject& entity) const override;

    std::unique_ptr<domain::IFileObject>
    fromSnapshot(const FileObjectSnapshot& snapshot) const override;
};
