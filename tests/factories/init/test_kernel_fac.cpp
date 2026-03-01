#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "factories/init/kernel_fac.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "domain/objects/file_object.h"
#include "domain/objects/directory.h"

using namespace domain;

TEST_CASE("KernelFactory createTabUserWithAdmin") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);

    KernelFactory factory(group_factory, inmem_factory, user_factory);

    auto table = factory.createTabUserWithAdmin(user_factory);

    REQUIRE(table != nullptr);
    REQUIRE(table->size() == 1);
    REQUIRE(table->contains("admin"));
}

TEST_CASE("KernelFactory createTabGroupWithAdmin") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);

    KernelFactory factory(group_factory, inmem_factory, user_factory);

    auto table = factory.createTabGroupWithAdmin(group_factory);

    REQUIRE(table != nullptr);
    REQUIRE(table->size() == 1);
    REQUIRE(table->contains("Admins"));
}

TEST_CASE("KernelFactory createEmptyTabUser") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);

    KernelFactory factory(group_factory, inmem_factory, user_factory);

    auto table = factory.createEmptyTabUser();

    REQUIRE(table != nullptr);
    REQUIRE(table->size() == 0);
}

TEST_CASE("KernelFactory createEmptyTabGroup") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);

    KernelFactory factory(group_factory, inmem_factory, user_factory);

    auto table = factory.createEmptyTabGroup();

    REQUIRE(table != nullptr);
    REQUIRE(table->size() == 0);
}

TEST_CASE("KernelFactory createDirRoot") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory factory(group_factory, inmem_factory, user_factory);

    auto root = factory.createDirRoot(dir_factory, inmem_factory, acl_factory, 0, 0);

    REQUIRE(root != nullptr);
    REQUIRE(root->getName() == "/");
    REQUIRE(root->getPath() == "/");
}

