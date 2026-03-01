#pragma once

#include <cstdint>
#include <string>
#include <vector>

class UserSnapshot {
public:
    UserSnapshot(
        std::uint64_t uid,
        std::string name,
        std::uint64_t main_group_id,
        std::vector<std::uint64_t> secondary_groups
    )
        : uid(uid),
          name(std::move(name)),
          main_group_id(main_group_id),
          secondary_groups(std::move(secondary_groups)) {}

    std::uint64_t getUid() const noexcept { return uid; }
    const std::string& getName() const noexcept { return name; }
    std::uint64_t getMainGroupId() const noexcept { return main_group_id; }
    const std::vector<std::uint64_t>& getSecondaryGroups() const noexcept {
        return secondary_groups;
    }

private:
    std::uint64_t uid;
    std::string name;
    std::uint64_t main_group_id;
    std::vector<std::uint64_t> secondary_groups;
};