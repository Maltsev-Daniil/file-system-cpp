//
// Created by nitsir on 12/28/25.
//

#include "mappers/file_system/file_system_mapper.h"

#include "containers/kernel_container.h"
#include "services/domain/kernel_service.h"
#include "domain/objects/directory.h"
#include "factories/init/kernel_fac.h"

#include "domain/entity/user.h"
#include "domain/entity/group.h"

FileSystemMapper::FileSystemMapper(
    const IUserMapper& user_mapper,
    const IGroupMapper& group_mapper,
    const IDirectoryMapper& directory_mapper
)
    : user_mapper_(user_mapper),
      group_mapper_(group_mapper),
      directory_mapper_(directory_mapper)
{}

FileSystemSnapshot
FileSystemMapper::toSnapshot(const domain::IDirectory& root_dir,
    const std::vector<const domain::IUser*>& users,
    const std::vector<const domain::IGroup*>& groups) const
{
    auto root_snapshot =
        directory_mapper_.toSnapshot(root_dir);

    std::vector<UserSnapshot> users_snap;
    for (const auto& user : users) {
        users_snap.push_back(user_mapper_.toSnapshot(*user));
    }

    std::vector<GroupSnapshot> groups_snap;
    for (const auto& group : groups) {
        groups_snap.push_back(group_mapper_.toSnapshot(*group));
    }

    return FileSystemSnapshot(
        std::move(root_snapshot),
        std::move(users_snap),
        std::move(groups_snap)
    );
}

std::shared_ptr<domain::IKernelContainer> FileSystemMapper::kernelFromSnapshot(
    const FileSystemSnapshot& snapshot,
    domain::IKernelFactory& kernel_factory) const
{
    auto users = usersFromSnapshot(snapshot);
    auto groups = groupsFromSnapshot(snapshot);
    auto root = rootFromSnapshot(snapshot);

    auto user_tab = kernel_factory.createEmptyTabUser();
    auto group_tab = kernel_factory.createEmptyTabGroup();

    for (auto& user : users) {
        user_tab->add(std::move(user));
    }

    for (auto& group : groups) {
        group_tab->add(std::move(group));
    }

    return kernel_factory.createKernelContainerFromSnapshot(
        std::move(user_tab),
        std::move(group_tab),
        std::move(root)
    );
}

std::vector<std::unique_ptr<domain::IUser>>
FileSystemMapper::usersFromSnapshot(
    const FileSystemSnapshot& snapshot) const
{
    std::vector<std::unique_ptr<domain::IUser>> users;
    users.reserve(snapshot.getUsers().size());

    for (const auto& user_snap : snapshot.getUsers()) {
        users.push_back(
            user_mapper_.fromSnapshot(user_snap)
        );
    }

    return users;
}

std::vector<std::unique_ptr<domain::IGroup>>
FileSystemMapper::groupsFromSnapshot(
    const FileSystemSnapshot& snapshot) const
{
    std::vector<std::unique_ptr<domain::IGroup>> groups;
    groups.reserve(snapshot.getGroups().size());

    for (const auto& group_snap : snapshot.getGroups()) {
        groups.push_back(
            group_mapper_.fromSnapshot(group_snap)
        );
    }

    return groups;
}

std::unique_ptr<domain::IDirectory>
FileSystemMapper::rootFromSnapshot(
    const FileSystemSnapshot& snapshot) const
{
    return directory_mapper_.fromSnapshot(
        snapshot.getRoot()
    );
}