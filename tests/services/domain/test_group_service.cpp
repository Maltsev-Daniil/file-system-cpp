#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/domain/group_service.h"
#include "factories/entity/group_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"
#include "factories/entity/user_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "factories/objects/file_object_fac.h"
#include "domain/entity/group.h"

using namespace services;
using namespace domain;

TEST_CASE("GroupService createGroup") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    GroupService service(group_factory, *kernel_container);

    auto group = service.createGroup("testgroup");
    REQUIRE(group != nullptr);
    REQUIRE(group->getGroupname() == "testgroup");
}

TEST_CASE("GroupService changeGroupName") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    GroupService service(group_factory, *kernel_container);

    auto group = service.createGroup("oldname");
    service.changeGroupName(group.get(), "newname");
    REQUIRE(group->getGroupname() == "newname");
}

TEST_CASE("GroupService changeGroupId") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    GroupService service(group_factory, *kernel_container);

    auto group = service.createGroup("testgroup");
    auto old_gid = group->getGid();
    service.changeGroupId(group.get(), 5000);
    REQUIRE(group->getGid() == 5000);
    REQUIRE(group->getGid() != old_gid);
}

TEST_CASE("GroupService addUserToGroup and removeUserFromGroup") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    GroupService service(group_factory, *kernel_container);

    auto group = service.createGroup("testgroup");
    service.addUserToGroup(group.get(), 1000);
    REQUIRE(service.isUserInGroup(group.get(), 1000));

    service.removeUserFromGroup(group.get(), 1000);
    REQUIRE_FALSE(service.isUserInGroup(group.get(), 1000));
}

TEST_CASE("GroupService addSubgroup and removeSubgroup") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    GroupService service(group_factory, *kernel_container);

    auto group = service.createGroup("testgroup");
    auto subgroup = service.createGroup("subgroup");
    auto subgroup_gid = subgroup->getGid();

    service.addSubgroup(group.get(), subgroup_gid);
    REQUIRE(service.isSubgroup(group.get(), subgroup_gid));

    service.removeSubgroup(group.get(), subgroup_gid);
    REQUIRE_FALSE(service.isSubgroup(group.get(), subgroup_gid));
}

TEST_CASE("GroupService getAllUsers and getAllSubgroups") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    GroupService service(group_factory, *kernel_container);

    auto group = service.createGroup("testgroup");
    service.addUserToGroup(group.get(), 1000);
    service.addUserToGroup(group.get(), 2000);

    auto users = service.getAllUsers(group.get());
    REQUIRE(users.size() == 2);
    REQUIRE(users.contains(1000));
    REQUIRE(users.contains(2000));

    auto subgroup = service.createGroup("subgroup");
    service.addSubgroup(group.get(), subgroup->getGid());

    auto subgroups = service.getAllSubgroups(group.get());
    REQUIRE(subgroups.size() == 1);
}

TEST_CASE("GroupService copyGroup") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    GroupService service(group_factory, *kernel_container);

    auto group = service.createGroup("testgroup");
    service.addUserToGroup(group.get(), 1000);

    auto copy = service.copyGroup(group.get());
    REQUIRE(copy != nullptr);
    REQUIRE(copy->getGroupname() == "testgroup");
    REQUIRE(copy->getUsers().size() == 1);
}

TEST_CASE("GroupService addGroupToTable") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    GroupService service(group_factory, *kernel_container);

    auto group = service.createGroup("testgroup");
    service.addGroupToTable(std::move(group));

    auto table = kernel_container->getGroupTable();
    REQUIRE(table->contains("testgroup"));
}

