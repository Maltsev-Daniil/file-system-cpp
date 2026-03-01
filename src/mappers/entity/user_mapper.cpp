//
// Created by nitsir on 12/28/25.
//
#include "mappers/entity/user_mapper.h"

#include <unordered_set>
#include <vector>

#include "domain/entity/user.h"
#include "snapshots/entity/user_snapshot.h"

UserSnapshot
UserMapper::toSnapshot(const domain::IUser& user) const
{
    std::vector<std::uint64_t> secondary_groups;
    secondary_groups.reserve(user.getSecondaryGroups().size());

    for (auto gid : user.getSecondaryGroups()) {
        secondary_groups.push_back(gid);
    }

    return UserSnapshot(
        user.getUid(),
        user.getUsername(),
        user.getGroup(),
        std::move(secondary_groups)
    );
}

std::unique_ptr<domain::IUser>
UserMapper::fromSnapshot(const UserSnapshot& snapshot) const
{
    std::unordered_set<domain::groupId_t> secondary_groups;
    secondary_groups.reserve(snapshot.getSecondaryGroups().size());

    for (auto gid : snapshot.getSecondaryGroups()) {
        secondary_groups.insert(gid);
    }

    return std::make_unique<domain::User>(
        snapshot.getUid(),
        snapshot.getName(),
        snapshot.getMainGroupId(),
        std::move(secondary_groups)
    );
}