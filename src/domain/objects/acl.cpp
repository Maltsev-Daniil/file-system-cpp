#include "../../../include/domain/objects/acl.h"
#include <memory>
#include <vector>

namespace domain {
void ACL::addUserPermission(userId_t uid, const IACLItem &item) {
    specific_user_rights[uid].insert(ACLItem(item));
}

AclResult ACL::removeUserPermission(userId_t uid, const IACLItem &item) {
    auto it = specific_user_rights.find(uid);
    if (it == specific_user_rights.end()) return AclResult::UserNotFound;

    size_t erased = it->second.erase(ACLItem(item));
    return erased ? AclResult::Success : AclResult::ItemNull;
}

const std::set<ACLItem> &ACL::getUserPermissions(userId_t uid) const {
    static const std::set<ACLItem> emptySet;
    auto it = specific_user_rights.find(uid);
    return it != specific_user_rights.end() ? it->second : emptySet;
}

bool ACL::hasUserPermission(userId_t uid, RightType perm, AccessType type) const {
    auto it = specific_user_rights.find(uid);
    if (it == specific_user_rights.end()) return false;
    return it->second.find(ACLItem(perm, type)) != it->second.end();
}

void ACL::addGroupPermission(groupId_t gid, const IACLItem &item) {
    specific_group_rights[gid].insert(ACLItem(item));
}

AclResult ACL::removeGroupPermission(groupId_t gid, const IACLItem &item) {
    auto it = specific_group_rights.find(gid);
    if (it == specific_group_rights.end()) return AclResult::GroupNotFound;

    size_t erased = it->second.erase(ACLItem(item));
    return erased ? AclResult::Success : AclResult::ItemNull;
}

const std::set<ACLItem> &ACL::getGroupPermissions(groupId_t gid) const {
    static const std::set<ACLItem> emptySet;
    auto it = specific_group_rights.find(gid);
    return it != specific_group_rights.end() ? it->second : emptySet;
}

bool ACL::hasGroupPermission(groupId_t gid, RightType perm, AccessType type) const {
    auto it = specific_group_rights.find(gid);
    if (it == specific_group_rights.end()) return false;
    return it->second.find(ACLItem(perm, type)) != it->second.end();
}

const std::set<ACLItem> &ACL::getOwnerUserMask() const {
    return owner_user_rights_mask;
}

const std::set<ACLItem> &ACL::getOwnerGroupMask() const {
    return owner_group_rights_mask;
}

const std::set<ACLItem> &ACL::getOtherMask() const {
    return other_rights_mask;
}

void ACL::setOwnerUserMask(const std::set<ACLItem> &new_mask) {
    owner_user_rights_mask.clear();
    owner_user_rights_mask.insert(new_mask.begin(), new_mask.end());
}

void ACL::setOwnerGroupMask(const std::set<ACLItem> &new_mask) {
    owner_group_rights_mask.clear();
    owner_group_rights_mask.insert(new_mask.begin(), new_mask.end());
}

void ACL::setOtherMask(const std::set<ACLItem> &new_mask) {
    other_rights_mask.clear();
    other_rights_mask.insert(new_mask.begin(), new_mask.end());
}

std::vector<std::pair<userId_t, std::set<ACLItem> > > ACL::getSpecUserPerms() const {
    std::vector<std::pair<userId_t, std::set<ACLItem> > > result;
    result.reserve(specific_user_rights.size());

    for (const auto &[uid, perms]: specific_user_rights) {
        result.emplace_back(uid, perms);
    }

    return result;
}

std::vector<std::pair<groupId_t, std::set<ACLItem> > > ACL::getSpecGroupPerms() const {
    std::vector<std::pair<groupId_t, std::set<ACLItem> > > result;
    result.reserve(specific_group_rights.size());

    for (const auto &[gid, perms]: specific_group_rights) {
        result.emplace_back(gid, perms);
    }

    return result;
}

std::unique_ptr<IACL> ACL::clone() const {
    auto copy = std::make_unique<ACL>(
        owner_user_rights_mask,
        owner_group_rights_mask,
        other_rights_mask
    );

    copy->specific_user_rights = specific_user_rights;
    copy->specific_group_rights = specific_group_rights;

    return copy;
}
}
