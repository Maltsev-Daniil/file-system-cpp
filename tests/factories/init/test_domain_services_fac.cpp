#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "factories/init/domain_services_fac.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"

using namespace services;
using namespace domain;

TEST_CASE("DomainServicesFactory createFileService") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto file_factory = std::make_shared<FileFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    DomainServicesFactory factory(
        file_factory,
        dir_factory,
        user_factory,
        group_factory,
        inmem_factory,
        acl_factory,
        kernel_container
    );

    auto file_service = factory.createFileService();
    REQUIRE(file_service != nullptr);
}

TEST_CASE("DomainServicesFactory createDirectoryService") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto file_factory = std::make_shared<FileFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    DomainServicesFactory factory(
        file_factory,
        dir_factory,
        user_factory,
        group_factory,
        inmem_factory,
        acl_factory,
        kernel_container
    );

    auto dir_service = factory.createDirectoryService();
    REQUIRE(dir_service != nullptr);
}

TEST_CASE("DomainServicesFactory createUserService") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto file_factory = std::make_shared<FileFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    DomainServicesFactory factory(
        file_factory,
        dir_factory,
        user_factory,
        group_factory,
        inmem_factory,
        acl_factory,
        kernel_container
    );

    auto user_service = factory.createUserService();
    REQUIRE(user_service != nullptr);
}

TEST_CASE("DomainServicesFactory createGroupService") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto file_factory = std::make_shared<FileFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    DomainServicesFactory factory(
        file_factory,
        dir_factory,
        user_factory,
        group_factory,
        inmem_factory,
        acl_factory,
        kernel_container
    );

    auto group_service = factory.createGroupService();
    REQUIRE(group_service != nullptr);
}

TEST_CASE("DomainServicesFactory createRightsService") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto file_factory = std::make_shared<FileFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    DomainServicesFactory factory(
        file_factory,
        dir_factory,
        user_factory,
        group_factory,
        inmem_factory,
        acl_factory,
        kernel_container
    );

    auto user_service = factory.createUserService();
    auto group_service = factory.createGroupService();
    auto rights_service = factory.createRightsService(user_service, group_service);
    REQUIRE(rights_service != nullptr);
}

TEST_CASE("DomainServicesFactory createKernelService") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto file_factory = std::make_shared<FileFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    DomainServicesFactory factory(
        file_factory,
        dir_factory,
        user_factory,
        group_factory,
        inmem_factory,
        acl_factory,
        kernel_container
    );

    auto kernel_service = factory.createKernelService();
    REQUIRE(kernel_service != nullptr);
}

TEST_CASE("DomainServicesFactory createSessionService") {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    auto user_factory = std::make_shared<UserFactory>(uid_gen);
    auto group_factory = std::make_shared<GroupFactory>(gid_gen);
    auto inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_factory = std::make_shared<ACLFactory>();
    auto file_factory = std::make_shared<FileFactory>();
    auto dir_factory = std::make_shared<DirectoryFactory>();

    KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
    auto kernel_container = kernel_factory.createKernelContainerColdStart(
        user_factory, group_factory, dir_factory, inmem_factory, acl_factory
    );

    DomainServicesFactory factory(
        file_factory,
        dir_factory,
        user_factory,
        group_factory,
        inmem_factory,
        acl_factory,
        kernel_container
    );

    auto session_service = factory.createSessionService();
    REQUIRE(session_service != nullptr);
}


