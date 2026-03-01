#pragma once
#include <memory>
#include <unordered_set>

#include "../types.h"

namespace domain {
class IUser {
public:
    virtual ~IUser() = default;

    virtual void setGroup(groupId_t new_gid) = 0;
    virtual void setUid(userId_t new_uid) = 0;
    virtual void setUsername(const std::string& new_username) = 0;

    virtual groupId_t getGroup() const = 0;
    virtual userId_t getUid() const = 0;
    virtual const std::string& getUsername() const = 0;

    virtual void addSecondaryGroup(groupId_t gid) = 0;
    virtual void removeSecondaryGroup(groupId_t gid) = 0;
    virtual const std::unordered_set<groupId_t>& getSecondaryGroups() const = 0;

    virtual std::unique_ptr<IUser> clone() const = 0;
};

class User : public IUser {
    userId_t uid;
    std::string user_name;
    groupId_t main_group;
    std::unordered_set<groupId_t> secondary_groups{};
public:
    User(userId_t user_id, const std::string& username, groupId_t main_group_id,
         const std::unordered_set<groupId_t>& secondary_groups_ids = {})
        : uid(user_id), user_name(username), main_group(main_group_id),
          secondary_groups(secondary_groups_ids) {}

    void setGroup(groupId_t new_gid) override {
        main_group = new_gid;
    }

    void setUid(userId_t new_uid) override {
        uid = new_uid;
    }

    void setUsername(const std::string& new_username) override {
        user_name = new_username;
    }

    groupId_t getGroup() const override {
        return main_group;
    }

    userId_t getUid() const override {
        return uid;
    }

    const std::string& getUsername() const override {
        return user_name;
    }

    void addSecondaryGroup(groupId_t gid) override {
        secondary_groups.insert(gid);
    }

    void removeSecondaryGroup(groupId_t gid) override {
        secondary_groups.erase(gid);
    }

    const std::unordered_set<groupId_t>& getSecondaryGroups() const override {
        return secondary_groups;
    }

    std::unique_ptr<IUser> clone() const override {
        return std::make_unique<User>(*this);
    }
};
}