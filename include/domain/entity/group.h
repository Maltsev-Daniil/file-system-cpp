#pragma once
#include <memory>
#include <string>
#include <unordered_set>

#include "../types.h"

namespace domain {
class IGroup {
public:
    virtual ~IGroup() = default;
    virtual void addUser(userId_t uid) = 0;
    virtual void delUser(userId_t uid) = 0;

    virtual void addGroup(groupId_t gid) = 0;
    virtual void delGroup(groupId_t gid) = 0;

    virtual groupId_t getGid() const = 0;
    virtual void setGid(groupId_t new_gid) = 0;

    virtual const std::string& getGroupname() const = 0;
    virtual void setGroupname(const std::string& new_groupname) = 0;

    virtual const std::unordered_set<userId_t>& getUsers() const = 0;
    virtual const std::unordered_set<groupId_t>& getSubGroups() const = 0;

    virtual std::unique_ptr<IGroup> clone() const = 0;
};

class Group : public IGroup {
    groupId_t gid;
    std::string group_name;
    std::unordered_set<userId_t> users{};
    std::unordered_set<groupId_t> sub_groups{};
public:
    Group(groupId_t group_id, const std::string& name)
        : gid(group_id), group_name(name) {}

    Group(const Group& other)
        : gid(other.gid), group_name(other.group_name),
          users(other.users), sub_groups(other.sub_groups) {}

    void addUser(userId_t uid) override {
        users.insert(uid);
    }

    void delUser(userId_t uid) override {
        users.erase(uid);
    }

    void addGroup(groupId_t gid) override {
        sub_groups.insert(gid);
    }

    void delGroup(groupId_t gid) override {
        sub_groups.erase(gid);
    }

    groupId_t getGid() const override {
        return gid;
    }

    void setGid(groupId_t new_gid) override {
        gid = new_gid;
    }

    const std::string& getGroupname() const override {
        return group_name;
    }

    void setGroupname(const std::string& new_groupname) override {
        group_name = new_groupname;
    }

    const std::unordered_set<userId_t>& getUsers() const override {
        return users;
    }

    const std::unordered_set<groupId_t>& getSubGroups() const override {
        return sub_groups;
    }

    std::unique_ptr<IGroup> clone() const override {
        return std::unique_ptr<IGroup>(new Group(*this));
    }
};
}
