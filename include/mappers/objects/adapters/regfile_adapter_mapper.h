#pragma once
#include <memory>
#include <typeindex>

#include "ifo_adapter_mapper.h"

namespace domain {
class IFileObject;
}

class IRegularFileMapper;
class FileObjectSnapshot;

class RegularFileMapperAdapter final : public IFileObjectMapperAdapter {
    IRegularFileMapper& mapper_;

public:
    explicit RegularFileMapperAdapter(IRegularFileMapper& mapper);

    std::type_index getEntityType() const override;

    std::type_index getSnapshotType() const override;

    std::unique_ptr<FileObjectSnapshot>
    toSnapshot(const domain::IFileObject& entity) const override;

    std::unique_ptr<domain::IFileObject>
    fromSnapshot(const FileObjectSnapshot& snapshot) const override;
};
