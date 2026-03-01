
#include "factories/objects/acl_fac.h"

#include <set>

#include "domain/objects/acl.h"

namespace domain {

static std::set<ACLItem> ownerMaskRootUser() {
    return {
        ACLItem(RightType::READ, AccessType::ALLOW),
        ACLItem(RightType::WRITE, AccessType::ALLOW),
        ACLItem(RightType::EXECUTE, AccessType::ALLOW),
        ACLItem(RightType::CHANGE_RIGHTS, AccessType::ALLOW),
        ACLItem(RightType::CHANGE_OWNER, AccessType::ALLOW)
    };
}

static std::set<ACLItem> ownerMaskRootGroup() {
    return {
        ACLItem(RightType::READ, AccessType::ALLOW),
        ACLItem(RightType::WRITE, AccessType::ALLOW),
        ACLItem(RightType::EXECUTE, AccessType::ALLOW),
        ACLItem(RightType::CHANGE_RIGHTS, AccessType::ALLOW)
    };
}

static std::set<ACLItem> otherMaskRoot() {
    return {
        ACLItem(RightType::READ, AccessType::ALLOW)
    };
}

static std::set<ACLItem> ownerMaskDefaultUser() {
    return {
        ACLItem(RightType::READ, AccessType::ALLOW),
        ACLItem(RightType::WRITE, AccessType::ALLOW)
    };
}

static std::set<ACLItem> ownerMaskDefaultGroup() {
    return {
        ACLItem(RightType::READ, AccessType::ALLOW)
    };
}

static std::set<ACLItem> otherMaskDefault() {
    return {
        ACLItem(RightType::READ, AccessType::ALLOW)
    };
}

std::unique_ptr<IACL> ACLFactory::createRootACL() {
    return std::make_unique<ACL>(
        ownerMaskRootUser(),
        ownerMaskRootGroup(),
        otherMaskRoot()
    );
}

std::unique_ptr<IACL> ACLFactory::createDefaultFileACL() {
    return std::make_unique<ACL>(
        ownerMaskDefaultUser(),
        ownerMaskDefaultGroup(),
        otherMaskDefault()
    );
}

std::unique_ptr<IACL> ACLFactory::createCustomACL(
    userId_t owner_uid,
    groupId_t owner_gid,
    const std::vector<ACLItem>& user_perms,
    const std::vector<ACLItem>& group_perms
) {
    auto acl = std::make_unique<ACL>(
        ownerMaskDefaultUser(),
        ownerMaskDefaultGroup(),
        otherMaskDefault()
    );

    for (const ACLItem& p : user_perms)
        acl->addUserPermission(owner_uid, p);

    for (const ACLItem& p : group_perms)
        acl->addGroupPermission(owner_gid, p);

    return acl;
}

std::unique_ptr<IACL> ACLFactory::createFullCustomACL(
    const std::set<ACLItem>& owner_user_mask,
    const std::set<ACLItem>& owner_group_mask,
    const std::set<ACLItem>& other_mask,
    const std::unordered_map<userId_t, std::set<ACLItem>>& user_rights,
    const std::unordered_map<groupId_t, std::set<ACLItem>>& group_rights
) {
    auto acl = std::make_unique<ACL>(
        owner_user_mask,
        owner_group_mask,
        other_mask
    );

    for (const auto& [uid, perms] : user_rights) {
        for (const ACLItem& item : perms) {
            acl->addUserPermission(uid, item);
        }
    }

    for (const auto& [gid, perms] : group_rights) {
        for (const ACLItem& item : perms) {
            acl->addGroupPermission(gid, item);
        }
    }

    return acl;
}

} // namespace domain