#include "domain/entity/user.h"
#include "domain/tables/user_table.h"
#include "services/domain/user_service.h"
#include "../../../include/factories/entity/user_fac.h"
#include <algorithm>
#include <stdexcept>
#include <string>

#include "containers/kernel_container.h"

using namespace domain;

namespace services {

std::unique_ptr<IUser> UserService::createUser(const std::string &username, domain::userId_t gid)
{
    return user_factory->createUser(username, gid);
}

void UserService::changeUserGroup(IUser* user, groupId_t new_group_id) {

    if (!user) {
        throw std::invalid_argument("changeUserGroup: user pointer is null");
    }
    const auto& secondary_groups = user->getSecondaryGroups();
    if (secondary_groups.find(new_group_id) != secondary_groups.end()) {
        user->removeSecondaryGroup(new_group_id);
    }
    user->setGroup(new_group_id);
}

void UserService::changeUsername(IUser* user, const std::string& new_username) {
    if (!user) {
        throw std::invalid_argument("changeUsername: user pointer is null");
    }
    if (new_username.empty()) {
        throw std::invalid_argument("changeUsername: username cannot be empty");
    }
    user->setUsername(new_username);
}

void UserService::changeUid(IUser* user, userId_t new_uid) {
    if (!user) {
        throw std::invalid_argument("changeUid: user pointer is null");
    }
    user->setUid(new_uid);
}

void UserService::addToSecondaryGroup(IUser* user, groupId_t group_id) {
    if (!user) {
        throw std::invalid_argument("addToSecondaryGroup: user pointer is null");
    }
    if (group_id == user->getGroup()) {
        throw std::invalid_argument("addToSecondaryGroup: cannot add main group as secondary group");
    }
    user->addSecondaryGroup(group_id);
}

void UserService::removeFromSecondaryGroup(IUser* user, groupId_t group_id) {
    if (!user) {
        throw std::invalid_argument("removeFromSecondaryGroup: user pointer is null");
    }
    if (!isUserInGroup(user, group_id))
        throw std::runtime_error("removeFromSecondaryGroup: user doesn't belong to group");
    user->removeSecondaryGroup(group_id);
}

bool UserService::isUserInGroup(IUser* user, groupId_t group_id) const {
    if (!user) {
        throw std::invalid_argument("isUserInGroup: user pointer is null");
    }
    
    if (user->getGroup() == group_id) {
        return true;
    }
    
    const auto& secondary_groups = user->getSecondaryGroups();
    return secondary_groups.find(group_id) != secondary_groups.end();
}

std::set<groupId_t> UserService::getAllUserGroups(IUser* user) const {
    if (!user) {
        throw std::invalid_argument("getAllUserGroups: user pointer is null");
    }
    
    std::set<groupId_t> all_groups;
    all_groups.insert(user->getGroup());
    
    const auto& secondary_groups = user->getSecondaryGroups();
    all_groups.insert(secondary_groups.begin(), secondary_groups.end());
    
    return all_groups;
}

std::unique_ptr<IUser> UserService::copyUser(IUser* user) const {
    if (!user) {
        throw std::invalid_argument("copyUser: user pointer is null");
    }
    if (!user_factory) {
        throw std::runtime_error("copyUser: user factory is not initialized");
    }
    
    std::unique_ptr<IUser> user_clone = user->clone();
    user_factory->generateId(user_clone.get());
    return user_clone;
}

void UserService::addUserToTable(std::unique_ptr<domain::IUser> user)
{
    if (!kernel_container_.getUserTable()->add(std::move(user)))
        throw std::runtime_error("addUserToTable: error in adding user");
}

} // namespace services