#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/domain/user_service.h"
#include "factories/entity/user_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "factories/objects/file_object_fac.h"
#include "domain/entity/user.h"

using namespace services;
using namespace domain;

TEST_CASE("UserService createUser") {
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

    UserService service(user_factory, *kernel_container);

    auto user = service.createUser("testuser", 2000);
    REQUIRE(user != nullptr);
    REQUIRE(user->getUsername() == "testuser");
    REQUIRE(user->getGroup() == 2000);
}

TEST_CASE("UserService changeUserGroup") {
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

    UserService service(user_factory, *kernel_container);

    auto user = service.createUser("testuser", 2000);
    service.changeUserGroup(user.get(), 3000);
    REQUIRE(user->getGroup() == 3000);
}

TEST_CASE("UserService changeUsername") {
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

    UserService service(user_factory, *kernel_container);

    auto user = service.createUser("oldname", 2000);
    service.changeUsername(user.get(), "newname");
    REQUIRE(user->getUsername() == "newname");
}

TEST_CASE("UserService changeUid") {
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

    UserService service(user_factory, *kernel_container);

    auto user = service.createUser("testuser", 2000);
    auto old_uid = user->getUid();
    service.changeUid(user.get(), 5000);
    REQUIRE(user->getUid() == 5000);
    REQUIRE(user->getUid() != old_uid);
}

TEST_CASE("UserService addToSecondaryGroup and removeFromSecondaryGroup") {
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

    UserService service(user_factory, *kernel_container);

    auto user = service.createUser("testuser", 2000);
    service.addToSecondaryGroup(user.get(), 3000);
    REQUIRE(service.isUserInGroup(user.get(), 3000));

    service.removeFromSecondaryGroup(user.get(), 3000);
    REQUIRE_FALSE(service.isUserInGroup(user.get(), 3000));
}

TEST_CASE("UserService getAllUserGroups") {
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

    UserService service(user_factory, *kernel_container);

    auto user = service.createUser("testuser", 2000);
    service.addToSecondaryGroup(user.get(), 3000);
    service.addToSecondaryGroup(user.get(), 4000);

    auto groups = service.getAllUserGroups(user.get());
    REQUIRE(groups.size() >= 2);
    REQUIRE(groups.contains(2000));
    REQUIRE(groups.contains(3000));
    REQUIRE(groups.contains(4000));
}

TEST_CASE("UserService copyUser") {
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

    UserService service(user_factory, *kernel_container);

    auto user = service.createUser("testuser", 2000);
    service.addToSecondaryGroup(user.get(), 3000);

    auto copy = service.copyUser(user.get());
    REQUIRE(copy != nullptr);
    REQUIRE(copy->getUsername() == "testuser");
    REQUIRE(copy->getGroup() == 2000);
    REQUIRE(copy->getSecondaryGroups().size() == 1);
}

TEST_CASE("UserService addUserToTable") {
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

    UserService service(user_factory, *kernel_container);

    auto user = service.createUser("testuser", 2000);
    service.addUserToTable(std::move(user));

    auto table = kernel_container->getUserTable();
    REQUIRE(table->contains("testuser"));
}

