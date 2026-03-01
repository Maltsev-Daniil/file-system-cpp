#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "mappers/acl/acl_mapper.h"
#include "factories/objects/acl_fac.h"
#include "domain/objects/acl.h"
#include "snapshots/acl/acl_snapshot.h"
#include "snapshots/acl/acl_item_snapshot.h"
#include "snapshots/acl/acl_subject_snapshot.h"

using namespace domain;

TEST_CASE("AclMapper toSnapshot basic") {
    ACLFactory factory;
    AclMapper mapper(factory);

    auto acl = factory.createRootACL();
    auto snapshot = mapper.toSnapshot(*acl);

    REQUIRE(snapshot.getItems().size() > 0);
}

TEST_CASE("AclMapper toSnapshot with all masks") {
    ACLFactory factory;
    AclMapper mapper(factory);

    std::set<ACLItem> owner_user = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::set<ACLItem> owner_group = {ACLItem(RightType::WRITE, AccessType::ALLOW)};
    std::set<ACLItem> other = {ACLItem(RightType::EXECUTE, AccessType::ALLOW)};

    auto acl = factory.createFullCustomACL(owner_user, owner_group, other, {}, {});
    auto snapshot = mapper.toSnapshot(*acl);

    REQUIRE(snapshot.getItems().size() == 3);
}

TEST_CASE("AclMapper toSnapshot with user and group specs") {
    ACLFactory factory;
    AclMapper mapper(factory);

    std::set<ACLItem> owner_user;
    std::set<ACLItem> owner_group;
    std::set<ACLItem> other;
    std::unordered_map<userId_t, std::set<ACLItem>> user_rights;
    std::unordered_map<groupId_t, std::set<ACLItem>> group_rights;

    user_rights[1000].insert(ACLItem(RightType::READ, AccessType::ALLOW));
    group_rights[2000].insert(ACLItem(RightType::WRITE, AccessType::ALLOW));

    auto acl = factory.createFullCustomACL(owner_user, owner_group, other, user_rights, group_rights);
    auto snapshot = mapper.toSnapshot(*acl);

    REQUIRE(snapshot.getItems().size() == 2);
}

TEST_CASE("AclMapper fromSnapshot basic") {
    ACLFactory factory;
    AclMapper mapper(factory);

    std::vector<AclItemSnapshot> items;
    items.emplace_back(
        AclSubjectSnapshot(AclSubjectKind::OwnerUser, 0),
        RightType::READ,
        AccessType::ALLOW
    );

    AclSnapshot snapshot(std::move(items));
    auto acl = mapper.fromSnapshot(snapshot);

    REQUIRE(acl != nullptr);
    REQUIRE(acl->getOwnerUserMask().size() == 1);
}

TEST_CASE("AclMapper fromSnapshot with all subject kinds") {
    ACLFactory factory;
    AclMapper mapper(factory);

    std::vector<AclItemSnapshot> items;
    items.emplace_back(
        AclSubjectSnapshot(AclSubjectKind::OwnerUser, 0),
        RightType::READ,
        AccessType::ALLOW
    );
    items.emplace_back(
        AclSubjectSnapshot(AclSubjectKind::OwnerGroup, 0),
        RightType::WRITE,
        AccessType::ALLOW
    );
    items.emplace_back(
        AclSubjectSnapshot(AclSubjectKind::Other, 0),
        RightType::EXECUTE,
        AccessType::ALLOW
    );
    items.emplace_back(
        AclSubjectSnapshot(AclSubjectKind::UserSpec, 1000),
        RightType::READ,
        AccessType::ALLOW
    );
    items.emplace_back(
        AclSubjectSnapshot(AclSubjectKind::GroupSpec, 2000),
        RightType::WRITE,
        AccessType::ALLOW
    );

    AclSnapshot snapshot(std::move(items));
    auto acl = mapper.fromSnapshot(snapshot);

    REQUIRE(acl != nullptr);
    REQUIRE(acl->getOwnerUserMask().size() == 1);
    REQUIRE(acl->getOwnerGroupMask().size() == 1);
    REQUIRE(acl->getOtherMask().size() == 1);
    REQUIRE(acl->getSpecUserPerms().size() == 1);
    REQUIRE(acl->getSpecGroupPerms().size() == 1);
}

TEST_CASE("AclMapper roundtrip") {
    ACLFactory factory;
    AclMapper mapper(factory);

    std::set<ACLItem> owner_user = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::set<ACLItem> owner_group = {ACLItem(RightType::WRITE, AccessType::ALLOW)};
    std::set<ACLItem> other = {ACLItem(RightType::EXECUTE, AccessType::ALLOW)};
    std::unordered_map<userId_t, std::set<ACLItem>> user_rights;
    std::unordered_map<groupId_t, std::set<ACLItem>> group_rights;

    user_rights[1000].insert(ACLItem(RightType::READ, AccessType::ALLOW));
    group_rights[2000].insert(ACLItem(RightType::WRITE, AccessType::ALLOW));

    auto original_acl = factory.createFullCustomACL(owner_user, owner_group, other, user_rights, group_rights);
    auto snapshot = mapper.toSnapshot(*original_acl);
    auto restored_acl = mapper.fromSnapshot(snapshot);

    REQUIRE(restored_acl->getOwnerUserMask().size() == 1);
    REQUIRE(restored_acl->getOwnerGroupMask().size() == 1);
    REQUIRE(restored_acl->getOtherMask().size() == 1);
    REQUIRE(restored_acl->getSpecUserPerms().size() == 1);
    REQUIRE(restored_acl->getSpecGroupPerms().size() == 1);
}

