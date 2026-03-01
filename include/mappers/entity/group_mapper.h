#pragma once

#include <memory>

namespace domain {
class IGroup;
}

class GroupSnapshot;

class IGroupMapper {
public:
    virtual ~IGroupMapper() = default;

    virtual GroupSnapshot
    toSnapshot(const domain::IGroup& group) const = 0;

    virtual std::unique_ptr<domain::IGroup>
    fromSnapshot(const GroupSnapshot& snapshot) const = 0;
};

class GroupMapper final : public IGroupMapper {
public:
    GroupSnapshot
    toSnapshot(const domain::IGroup& group) const override;

    std::unique_ptr<domain::IGroup>
    fromSnapshot(const GroupSnapshot& snapshot) const override;
};