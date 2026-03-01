//
// Created by nitsir on 12/28/25.
//

#include "mappers/entity/group_mapper.h"

#include <unordered_set>
#include <vector>

#include "domain/entity/group.h"
#include "snapshots/entity/group_snapshot.h"

GroupSnapshot
GroupMapper::toSnapshot(const domain::IGroup& group) const
{
    std::vector<std::uint64_t> users;
    users.reserve(group.getUsers().size());

    for (auto uid : group.getUsers()) {
        users.push_back(uid);
    }

    std::vector<std::uint64_t> sub_groups;
    sub_groups.reserve(group.getSubGroups().size());

    for (auto gid : group.getSubGroups()) {
        sub_groups.push_back(gid);
    }

    return GroupSnapshot(
        group.getGid(),
        group.getGroupname(),
        std::move(users),
        std::move(sub_groups)
    );
}

std::unique_ptr<domain::IGroup>
GroupMapper::fromSnapshot(const GroupSnapshot& snapshot) const
{
    auto group = std::make_unique<domain::Group>(
        snapshot.getGid(),
        snapshot.getName()
    );

    for (auto uid : snapshot.getUsers()) {
        group->addUser(uid);
    }

    for (auto gid : snapshot.getSubGroups()) {
        group->addGroup(gid);
    }

    return group;
}