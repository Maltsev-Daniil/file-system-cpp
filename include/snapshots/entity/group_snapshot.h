#pragma once

#include <cstdint>
#include <string>
#include <vector>

class GroupSnapshot {
public:
    GroupSnapshot(
        std::uint64_t gid,
        std::string name,
        std::vector<std::uint64_t> users,
        std::vector<std::uint64_t> sub_groups
    )
        : gid(gid),
          name(std::move(name)),
          users(std::move(users)),
          sub_groups(std::move(sub_groups)) {}

    std::uint64_t getGid() const noexcept { return gid; }
    const std::string& getName() const noexcept { return name; }
    const std::vector<std::uint64_t>& getUsers() const noexcept { return users; }
    const std::vector<std::uint64_t>& getSubGroups() const noexcept {
        return sub_groups;
    }

private:
    std::uint64_t gid;
    std::string name;
    std::vector<std::uint64_t> users;
    std::vector<std::uint64_t> sub_groups;
};