#include "services/domain/group_service.h"
#include "domain/types.h"
#include "domain/entity/group.h"
#include "domain/tables/itable.h"
#include <cassert>
#include <stdexcept>

#include "../../../include/factories/entity/group_fac.h"
#include "containers/kernel_container.h"

using namespace domain;

namespace services {

std::unique_ptr<domain::IGroup> GroupService::createGroup(const std::string &groupname)
{
    return group_factory->createGroup(groupname);
}

void GroupService::changeGroupName(IGroup* group, const std::string& new_name) {

    if (!group) {
        throw std::invalid_argument("changeGroupName: group pointer is null");
    }
    if (new_name.empty()) {
        throw std::invalid_argument("changeGroupName: group name cannot be empty");
    }
    group->setGroupname(new_name);
}

void GroupService::changeGroupId(IGroup* group, groupId_t new_gid) {
    if (!group) {
        throw std::invalid_argument("changeGroupId: group pointer is null");
    }
    assert(!isSubgroup(group, new_gid));
    group->setGid(new_gid);
}

void GroupService::addUserToGroup(IGroup* group, userId_t user_id) {
    if (!group) {
        throw std::invalid_argument("addUserToGroup: group pointer is null");
    }
    if (isUserInGroup(group, user_id)) {
        throw std::runtime_error("addUserToGroup: user already in group");
    }
    group->addUser(user_id);
}

void GroupService::removeUserFromGroup(IGroup* group, userId_t user_id) {
    if (!group) {
        throw std::invalid_argument("removeUserFromGroup: group pointer is null");
    }
    if (!isUserInGroup(group, user_id)) {
        throw std::runtime_error("removeUserFromGroup: user not in group");
    }
    group->delUser(user_id);
}

void GroupService::addSubgroup(IGroup* group, groupId_t subgroup_id) {
    if (!group) {
        throw std::invalid_argument("addSubgroup: group pointer is null");
    }
    if (subgroup_id == group->getGid()) {
        throw std::invalid_argument("addSubgroup: cannot add group to itself");
    }
    if (isSubgroup(group, subgroup_id)) {
        throw std::runtime_error("addSubgroup: subgroup already exists");
    }
    group->addGroup(subgroup_id);
}

void GroupService::removeSubgroup(IGroup* group, groupId_t subgroup_id) {
    if (!group) {
        throw std::invalid_argument("removeSubgroup: group pointer is null");
    }
    if (!isSubgroup(group, subgroup_id)) {
        throw std::runtime_error("removeSubgroup: subgroup not found");
    }
    group->delGroup(subgroup_id);
}

bool GroupService::isUserInGroup(IGroup* group, userId_t user_id) const {
    if (!group) {
        throw std::invalid_argument("isUserInGroup: group pointer is null");
    }
    
    const auto& users = group->getUsers();
    return users.find(user_id) != users.end();
}

bool GroupService::isSubgroup(IGroup* group, groupId_t subgroup_id) const {
    if (!group) {
        throw std::invalid_argument("isSubgroup: group pointer is null");
    }
    
    const auto& subgroups = group->getSubGroups();
    return subgroups.find(subgroup_id) != subgroups.end();
}

std::set<userId_t> GroupService::getAllUsers(IGroup* group) const {
    if (!group) {
        throw std::invalid_argument("getAllUsers: group pointer is null");
    }
    
    const auto& users = group->getUsers();
    return std::set<userId_t>(users.begin(), users.end());
}

std::set<groupId_t> GroupService::getAllSubgroups(IGroup* group) const {
    if (!group) {
        throw std::invalid_argument("getAllSubgroups: group pointer is null");
    }
    
    const auto& subgroups = group->getSubGroups();
    return std::set<groupId_t>(subgroups.begin(), subgroups.end());
}

std::unique_ptr<IGroup> GroupService::copyGroup(IGroup* group) const {
    if (!group) {
        throw std::invalid_argument("copyGroup: group pointer is null");
    }
    if (!group_factory) {
        throw std::runtime_error("copyGroup: group factory is not initialized");
    }
    
    std::unique_ptr<IGroup> group_clone = group->clone();
    group_factory->generateId(group_clone.get());
    return group_clone;
}

void GroupService::addGroupToTable(std::unique_ptr<IGroup> group)
{
    if (!kernel_container_.getGroupTable()->add(std::move(group)))
        throw std::runtime_error("addGroupToTable: error in adding group");
}

} // namespace services