#include "services/domain/rights_service.h"
#include "domain/entity/user.h"
#include "services/domain/user_service.h"
#include "../../../include/domain/objects/acl.h"
#include <algorithm>
#include <stdexcept>

#include "domain/objects/file_object.h"
#include "domain/objects/inmem_file.h"

using namespace domain;

namespace services {

bool RightsService::hasRight(IUser* user, IFileObject* file_object, RightType right) {
    if (!user) throw std::invalid_argument("hasRight: user pointer is null");
    if (!file_object) throw std::invalid_argument("hasRight: file object pointer is null");

    userId_t uid = user->getUid();

    if (uid == 0) return true; // root

    userId_t owner_uid = file_object->getInMemoryFile()->getUid();
    groupId_t owner_gid = file_object->getInMemoryFile()->getGid();
    std::set<groupId_t> user_groups = getAllGroupsForUser(user);

    const IACL* acl = file_object->getInMemoryFile()->getAcl();

    if (owner_uid == uid) {
        for (const auto& e : acl->getOwnerUserMask())
            if (e.getPerm() == right && e.getType() == AccessType::DENY)
                return false;
        for (const auto& e : acl->getOwnerUserMask())
            if (e.getPerm() == right && e.getType() == AccessType::ALLOW)
                return true;
    }

    if (user_groups.count(owner_gid) > 0) {
        for (const auto& e : acl->getOwnerGroupMask())
            if (e.getPerm() == right && e.getType() == AccessType::DENY)
                return false;
        for (const auto& e : acl->getOwnerGroupMask())
            if (e.getPerm() == right && e.getType() == AccessType::ALLOW)
                return true;
    }

    if (acl->hasUserPermission(uid, right, AccessType::DENY))
        return false;

    for (auto gid : user_groups)
        if (acl->hasGroupPermission(gid, right, AccessType::DENY))
            return false;

    if (acl->hasUserPermission(uid, right, AccessType::ALLOW))
        return true;

    for (auto gid : user_groups)
        if (acl->hasGroupPermission(gid, right, AccessType::ALLOW))
            return true;

    for (const auto& e : acl->getOtherMask())
        if (e.getPerm() == right && e.getType() == AccessType::DENY)
            return false;

    for (const auto& e : acl->getOtherMask())
        if (e.getPerm() == right && e.getType() == AccessType::ALLOW)
            return true;

    return false;
}


std::set<RightType> RightsService::getEffectiveRights(IUser* user, IFileObject* file_object) {
    if (!user) throw std::invalid_argument("getEffectiveRights: user pointer is null");
    if (!file_object) throw std::invalid_argument("getEffectiveRights: file object pointer is null");

    userId_t uid = user->getUid();
    userId_t owner_uid = file_object->getInMemoryFile()->getUid();
    groupId_t owner_gid = file_object->getInMemoryFile()->getGid();
    std::set<groupId_t> user_groups = getAllGroupsForUser(user);

    const IACL* acl = file_object->getInMemoryFile()->getAcl();

    std::set<RightType> allowed;
    std::set<RightType> denied;

    if (owner_uid == uid) {
        for (const auto& e : acl->getOwnerUserMask()) {
            if (e.getType() == AccessType::DENY) denied.insert(e.getPerm());
            else allowed.insert(e.getPerm());
        }
    }

    if (user_groups.count(owner_gid) > 0) {
        for (const auto& e : acl->getOwnerGroupMask()) {
            if (e.getType() == AccessType::DENY) denied.insert(e.getPerm());
            else allowed.insert(e.getPerm());
        }
    }

    for (const auto& e : acl->getUserPermissions(uid)) {
        if (e.getType() == AccessType::DENY) denied.insert(e.getPerm());
        else allowed.insert(e.getPerm());
    }

    for (auto gid : user_groups) {
        for (const auto& e : acl->getGroupPermissions(gid)) {
            if (e.getType() == AccessType::DENY) denied.insert(e.getPerm());
            else allowed.insert(e.getPerm());
        }
    }

    for (const auto& e : acl->getOtherMask()) {
        if (e.getType() == AccessType::DENY) denied.insert(e.getPerm());
        else allowed.insert(e.getPerm());
    }

    std::set<RightType> effective;
    for (auto r : allowed)
        if (!(denied.count(r)>0))
            effective.insert(r);

    return effective;
}

bool RightsService::hasGlobalRight(domain::IUser *user, domain::RightType right)
{
    switch (right) {
        case domain::RightType::USER_MODIFY:
            return user->getUid() == 0;
        default:
            return false;
    }
    return false;
}

std::set<groupId_t> RightsService::getAllGroupsForUser(IUser* user) const {
    return user_service.getAllUserGroups(user);
}

} // namespace services