#pragma once

#include <memory>
#include <vector>

#include "snapshots/entity/group_snapshot.h"
#include "snapshots/entity/user_snapshot.h"
#include "snapshots/objects/directory_snapshot.h"

class FileSystemSnapshot {
public:
    FileSystemSnapshot(
        std::unique_ptr<DirectorySnapshot> root,
        std::vector<UserSnapshot> users,
        std::vector<GroupSnapshot> groups
    )
        : root(std::move(root)),
          users(std::move(users)),
          groups(std::move(groups)) {}

    const DirectorySnapshot& getRoot() const noexcept { return *root; }
    const std::vector<UserSnapshot>& getUsers() const noexcept { return users; }
    const std::vector<GroupSnapshot>& getGroups() const noexcept { return groups; }

private:
    std::unique_ptr<DirectorySnapshot> root;
    std::vector<UserSnapshot> users;
    std::vector<GroupSnapshot> groups;
};
