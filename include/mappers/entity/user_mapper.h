#pragma once
#include <memory>

#include "snapshots/entity/user_snapshot.h"

namespace domain {
class IUser;
}

class IUserMapper {
public:
    virtual ~IUserMapper() = default;

    virtual UserSnapshot
    toSnapshot(const domain::IUser& user) const = 0;

    virtual std::unique_ptr<domain::IUser>
    fromSnapshot(const UserSnapshot& snapshot) const = 0;
};

class UserMapper final : public IUserMapper {
public:
    UserSnapshot
    toSnapshot(const domain::IUser& user) const override;

    std::unique_ptr<domain::IUser>
    fromSnapshot(const UserSnapshot& snapshot) const override;
};