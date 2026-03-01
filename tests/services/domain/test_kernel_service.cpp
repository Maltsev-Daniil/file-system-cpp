#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/domain/kernel_service.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"
#include "domain/objects/directory.h"
#include "domain/objects/inmem_file.h"
#include "domain/entity/user.h"
#include "domain/entity/group.h"

using namespace services;
using namespace domain;

TEST_CASE("KernelService resolvePath root") {
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

    KernelService service(kernel_container, inmem_factory);

    auto* root = service.resolvePath("/");
    REQUIRE(root != nullptr);
    REQUIRE(root->getName() == "/");
}

TEST_CASE("KernelService splitPath") {
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

    KernelService service(kernel_container, inmem_factory);

    auto parts = service.splitPath("/home/user/file.txt");
    REQUIRE(parts.size() == 3);
    REQUIRE(parts[0] == "home");
    REQUIRE(parts[1] == "user");
    REQUIRE(parts[2] == "file.txt");
}

TEST_CASE("KernelService normalizePathParts") {
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

    KernelService service(kernel_container, inmem_factory);

    std::vector<std::string> parts = {"home", ".", "..", "user", "file.txt"};
    auto normalized = service.normalizePathParts(parts);

    REQUIRE(normalized.size() == 2);
    REQUIRE(normalized[0] == "user");
    REQUIRE(normalized[1] == "file.txt");
}

TEST_CASE("KernelService matchName") {
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

    KernelService service(kernel_container, inmem_factory);

    REQUIRE(service.matchName("test*", "testfile"));
    REQUIRE(service.matchName("*file", "testfile"));
    REQUIRE(service.matchName("test", "test"));
    REQUIRE_FALSE(service.matchName("test", "other"));
}

TEST_CASE("KernelService resolveUser and resolveGroup") {
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

    auto user = user_factory->createUser("testuser", 2000);
    kernel_container->getUserTable()->add(std::move(user));

    auto group = group_factory->createGroup("testgroup");
    kernel_container->getGroupTable()->add(std::move(group));

    KernelService service(kernel_container, inmem_factory);

    auto* resolved_user = service.resolveUser("testuser");
    REQUIRE(resolved_user != nullptr);
    REQUIRE(resolved_user->getUsername() == "testuser");

    auto* resolved_group = service.resolveGroup("testgroup");
    REQUIRE(resolved_group != nullptr);
    REQUIRE(resolved_group->getGroupname() == "testgroup");
}

TEST_CASE("KernelService getRootDir") {
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

    KernelService service(kernel_container, inmem_factory);

    auto* root = service.getRootDir();
    REQUIRE(root != nullptr);
    REQUIRE(root->getName() == "/");
}

TEST_CASE("KernelService getInMemory") {
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

    KernelService service(kernel_container, inmem_factory);

    auto* root = service.getRootDir();
    auto* inmem = service.getInMemory(root);
    REQUIRE(inmem != nullptr);
}

