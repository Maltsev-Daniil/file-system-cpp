#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "factories/init/app_services_fac.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "services/domain/rights_service.h"
#include "services/domain/kernel_service.h"
#include "services/domain/file_service.h"
#include "services/domain/dir_service.h"
#include "services/session/session_service.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"

using namespace services;
using namespace domain;

TEST_CASE("AppFactory createFileAppService") {
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

    auto user_service = std::make_shared<UserService>(user_factory, *kernel_container);
    auto group_service = std::make_shared<GroupService>(group_factory, *kernel_container);
    auto rights_service = std::make_shared<RightsService>(*user_service, *group_service);
    auto kernel_service = std::make_shared<KernelService>(kernel_container, inmem_factory);
    auto file_service = std::make_shared<FileService>(file_factory, inmem_factory, acl_factory);
    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    SessionService session_service;

    AppFactory factory(kernel_service, rights_service, file_service, dir_service, user_service, group_service, session_service);

    auto app_file_service = factory.createFileAppService();
    REQUIRE(app_file_service != nullptr);
}

TEST_CASE("AppFactory createDirectoryAppService") {
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

    auto user_service = std::make_shared<UserService>(user_factory, *kernel_container);
    auto group_service = std::make_shared<GroupService>(group_factory, *kernel_container);
    auto rights_service = std::make_shared<RightsService>(*user_service, *group_service);
    auto kernel_service = std::make_shared<KernelService>(kernel_container, inmem_factory);
    auto file_service = std::make_shared<FileService>(file_factory, inmem_factory, acl_factory);
    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    SessionService session_service;

    AppFactory factory(kernel_service, rights_service, file_service, dir_service, user_service, group_service, session_service);

    auto app_dir_service = factory.createDirectoryAppService();
    REQUIRE(app_dir_service != nullptr);
}

TEST_CASE("AppFactory createUserAppService") {
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

    auto user_service = std::make_shared<UserService>(user_factory, *kernel_container);
    auto group_service = std::make_shared<GroupService>(group_factory, *kernel_container);
    auto rights_service = std::make_shared<RightsService>(*user_service, *group_service);
    auto kernel_service = std::make_shared<KernelService>(kernel_container, inmem_factory);
    auto file_service = std::make_shared<FileService>(file_factory, inmem_factory, acl_factory);
    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    SessionService session_service;

    AppFactory factory(kernel_service, rights_service, file_service, dir_service, user_service, group_service, session_service);

    auto app_user_service = factory.createUserAppService();
    REQUIRE(app_user_service != nullptr);
}

TEST_CASE("AppFactory createGroupAppService") {
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

    auto user_service = std::make_shared<UserService>(user_factory, *kernel_container);
    auto group_service = std::make_shared<GroupService>(group_factory, *kernel_container);
    auto rights_service = std::make_shared<RightsService>(*user_service, *group_service);
    auto kernel_service = std::make_shared<KernelService>(kernel_container, inmem_factory);
    auto file_service = std::make_shared<FileService>(file_factory, inmem_factory, acl_factory);
    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    SessionService session_service;

    AppFactory factory(kernel_service, rights_service, file_service, dir_service, user_service, group_service, session_service);

    auto app_group_service = factory.createGroupAppService();
    REQUIRE(app_group_service != nullptr);
}

TEST_CASE("AppFactory createRightsAppService") {
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

    auto user_service = std::make_shared<UserService>(user_factory, *kernel_container);
    auto group_service = std::make_shared<GroupService>(group_factory, *kernel_container);
    auto rights_service = std::make_shared<RightsService>(*user_service, *group_service);
    auto kernel_service = std::make_shared<KernelService>(kernel_container, inmem_factory);
    auto file_service = std::make_shared<FileService>(file_factory, inmem_factory, acl_factory);
    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    SessionService session_service;

    AppFactory factory(kernel_service, rights_service, file_service, dir_service, user_service, group_service, session_service);

    auto app_rights_service = factory.createRightsAppService();
    REQUIRE(app_rights_service != nullptr);
}


