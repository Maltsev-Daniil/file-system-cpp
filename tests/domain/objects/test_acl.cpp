#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/objects/acl.h"
#include "factories/objects/acl_fac.h"

using namespace domain;

TEST_CASE("ACLItem basic") {
    ACLItem item(RightType::READ, AccessType::ALLOW);
    REQUIRE(item.getPerm() == RightType::READ);
    REQUIRE(item.getType() == AccessType::ALLOW);

    item.setPerm(RightType::WRITE);
    item.setType(AccessType::DENY);
    REQUIRE(item.getPerm() == RightType::WRITE);
    REQUIRE(item.getType() == AccessType::DENY);
}

TEST_CASE("ACLItem copy") {
    ACLItem item1(RightType::READ, AccessType::ALLOW);
    ACLItem item2(item1);
    REQUIRE(item2.getPerm() == RightType::READ);
    REQUIRE(item2.getType() == AccessType::ALLOW);
}

TEST_CASE("ACLItem comparison") {
    ACLItem item1(RightType::READ, AccessType::ALLOW);
    ACLItem item2(RightType::WRITE, AccessType::ALLOW);
    REQUIRE(item1 < item2);
}

TEST_CASE("ACL basic creation") {
    std::set<ACLItem> owner_user = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::set<ACLItem> owner_group = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::set<ACLItem> other = {ACLItem(RightType::READ, AccessType::ALLOW)};

    ACL acl(owner_user, owner_group, other);
    REQUIRE(acl.getOwnerUserMask().size() == 1);
    REQUIRE(acl.getOwnerGroupMask().size() == 1);
    REQUIRE(acl.getOtherMask().size() == 1);
}

TEST_CASE("ACL add user permission") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    ACLItem item(RightType::READ, AccessType::ALLOW);
    acl.addUserPermission(100, item);

    auto perms = acl.getUserPermissions(100);
    REQUIRE(perms.size() == 1);
    REQUIRE(perms.count(item) == 1);
}

TEST_CASE("ACL add group permission") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    ACLItem item(RightType::WRITE, AccessType::ALLOW);
    acl.addGroupPermission(200, item);

    auto perms = acl.getGroupPermissions(200);
    REQUIRE(perms.size() == 1);
    REQUIRE(perms.count(item) == 1);
}

TEST_CASE("ACL remove user permission") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    ACLItem item(RightType::READ, AccessType::ALLOW);
    acl.addUserPermission(100, item);
    REQUIRE(acl.getUserPermissions(100).size() == 1);

    auto result = acl.removeUserPermission(100, item);
    REQUIRE(result == AclResult::Success);
    REQUIRE(acl.getUserPermissions(100).size() == 0);
}

TEST_CASE("ACL remove group permission") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    ACLItem item(RightType::WRITE, AccessType::ALLOW);
    acl.addGroupPermission(200, item);
    REQUIRE(acl.getGroupPermissions(200).size() == 1);

    auto result = acl.removeGroupPermission(200, item);
    REQUIRE(result == AclResult::Success);
    REQUIRE(acl.getGroupPermissions(200).size() == 0);
}

TEST_CASE("ACL has user permission") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    ACLItem item(RightType::READ, AccessType::ALLOW);
    acl.addUserPermission(100, item);

    REQUIRE(acl.hasUserPermission(100, RightType::READ, AccessType::ALLOW));
    REQUIRE_FALSE(acl.hasUserPermission(100, RightType::WRITE, AccessType::ALLOW));
}

TEST_CASE("ACL has group permission") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    ACLItem item(RightType::WRITE, AccessType::ALLOW);
    acl.addGroupPermission(200, item);

    REQUIRE(acl.hasGroupPermission(200, RightType::WRITE, AccessType::ALLOW));
    REQUIRE_FALSE(acl.hasGroupPermission(200, RightType::READ, AccessType::ALLOW));
}

TEST_CASE("ACL clear user permissions") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    acl.addUserPermission(100, ACLItem(RightType::READ, AccessType::ALLOW));
    acl.addUserPermission(100, ACLItem(RightType::WRITE, AccessType::ALLOW));
    REQUIRE(acl.getUserPermissions(100).size() == 2);

    acl.clearUserPermissions(100);
    REQUIRE(acl.getUserPermissions(100).size() == 0);
}

TEST_CASE("ACL clear group permissions") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    acl.addGroupPermission(200, ACLItem(RightType::READ, AccessType::ALLOW));
    acl.addGroupPermission(200, ACLItem(RightType::WRITE, AccessType::ALLOW));
    REQUIRE(acl.getGroupPermissions(200).size() == 2);

    acl.clearGroupPermissions(200);
    REQUIRE(acl.getGroupPermissions(200).size() == 0);
}

TEST_CASE("ACL set masks") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    std::set<ACLItem> new_owner_user = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::set<ACLItem> new_owner_group = {ACLItem(RightType::WRITE, AccessType::ALLOW)};
    std::set<ACLItem> new_other = {ACLItem(RightType::EXECUTE, AccessType::ALLOW)};

    acl.setOwnerUserMask(new_owner_user);
    acl.setOwnerGroupMask(new_owner_group);
    acl.setOtherMask(new_other);

    REQUIRE(acl.getOwnerUserMask().size() == 1);
    REQUIRE(acl.getOwnerGroupMask().size() == 1);
    REQUIRE(acl.getOtherMask().size() == 1);
}

TEST_CASE("ACL get spec user perms") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    acl.addUserPermission(100, ACLItem(RightType::READ, AccessType::ALLOW));
    acl.addUserPermission(200, ACLItem(RightType::WRITE, AccessType::ALLOW));

    auto spec_perms = acl.getSpecUserPerms();
    REQUIRE(spec_perms.size() == 2);
}

TEST_CASE("ACL get spec group perms") {
    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    ACL acl(owner_user, owner_group, other);

    acl.addGroupPermission(100, ACLItem(RightType::READ, AccessType::ALLOW));
    acl.addGroupPermission(200, ACLItem(RightType::WRITE, AccessType::ALLOW));

    auto spec_perms = acl.getSpecGroupPerms();
    REQUIRE(spec_perms.size() == 2);
}

TEST_CASE("ACL clone") {
    std::set<ACLItem> owner_user = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::set<ACLItem> owner_group = {ACLItem(RightType::WRITE, AccessType::ALLOW)};
    std::set<ACLItem> other = {ACLItem(RightType::EXECUTE, AccessType::ALLOW)};
    ACL acl(owner_user, owner_group, other);

    acl.addUserPermission(100, ACLItem(RightType::READ, AccessType::ALLOW));

    auto cloned = acl.clone();
    REQUIRE(cloned->getOwnerUserMask().size() == 1);
    REQUIRE(cloned->getUserPermissions(100).size() == 1);
}

TEST_CASE("ACLFactory createRootACL") {
    ACLFactory factory;
    auto acl = factory.createRootACL();
    REQUIRE(acl != nullptr);
    REQUIRE(acl->getOwnerUserMask().size() > 0);
}

TEST_CASE("ACLFactory createDefaultFileACL") {
    ACLFactory factory;
    auto acl = factory.createDefaultFileACL();
    REQUIRE(acl != nullptr);
    REQUIRE(acl->getOwnerUserMask().size() > 0);
}

TEST_CASE("ACLFactory createCustomACL") {
    ACLFactory factory;
    std::vector<ACLItem> user_perms = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::vector<ACLItem> group_perms = {ACLItem(RightType::WRITE, AccessType::ALLOW)};

    auto acl = factory.createCustomACL(100, 200, user_perms, group_perms);
    REQUIRE(acl != nullptr);
    REQUIRE(acl->hasUserPermission(100, RightType::READ, AccessType::ALLOW));
    REQUIRE(acl->hasGroupPermission(200, RightType::WRITE, AccessType::ALLOW));
}

TEST_CASE("ACLFactory createFullCustomACL") {
    ACLFactory factory;
    std::set<ACLItem> owner_user = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::set<ACLItem> owner_group = {ACLItem(RightType::WRITE, AccessType::ALLOW)};
    std::set<ACLItem> other = {ACLItem(RightType::EXECUTE, AccessType::ALLOW)};
    std::unordered_map<userId_t, std::set<ACLItem>> user_rights = {
        {100, {ACLItem(RightType::READ, AccessType::ALLOW)}}
    };
    std::unordered_map<groupId_t, std::set<ACLItem>> group_rights = {
        {200, {ACLItem(RightType::WRITE, AccessType::ALLOW)}}
    };

    auto acl = factory.createFullCustomACL(owner_user, owner_group, other, user_rights, group_rights);
    REQUIRE(acl != nullptr);
    REQUIRE(acl->getOwnerUserMask().size() == 1);
    REQUIRE(acl->hasUserPermission(100, RightType::READ, AccessType::ALLOW));
    REQUIRE(acl->hasGroupPermission(200, RightType::WRITE, AccessType::ALLOW));
}

