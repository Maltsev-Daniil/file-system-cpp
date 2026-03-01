#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/application/app_dir_service.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "services/domain/rights_service.h"
#include "services/domain/kernel_service.h"
#include "services/domain/dir_service.h"
#include "services/session/session_service.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"
#include "dto/objects/dir_content_dto.h"
#include "domain/objects/directory.h"
#include "domain/objects/directory_structure.h"

using namespace services;
using namespace domain;

TEST_CASE("DirectoryAppService createDirectory") {
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

    auto user_service = std::make_shared<UserService>(user_factory, *kernel_container);
    auto group_service = std::make_shared<GroupService>(group_factory, *kernel_container);
    auto rights_service = std::make_shared<RightsService>(*user_service, *group_service);
    auto kernel_service = std::make_shared<KernelService>(kernel_container, inmem_factory);

    SessionService session_service;
    session_service.login("admin", 0, "/");

    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    DirectoryAppService app_service(rights_service, dir_service, kernel_service, session_service);

    app_service.createDirectory("testdir", "/");

    auto* root = kernel_service->resolvePath("/");
    auto* dir = dynamic_cast<IDirectory*>(root);
    REQUIRE(dir != nullptr);
    REQUIRE(dir->getDirectoryStructure()->contains("testdir"));
}

TEST_CASE("DirectoryAppService listDirectory") {
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

    auto user_service = std::make_shared<UserService>(user_factory, *kernel_container);
    auto group_service = std::make_shared<GroupService>(group_factory, *kernel_container);
    auto rights_service = std::make_shared<RightsService>(*user_service, *group_service);
    auto kernel_service = std::make_shared<KernelService>(kernel_container, inmem_factory);

    SessionService session_service;
    session_service.login("admin", 0, "/");

    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    DirectoryAppService app_service(rights_service, dir_service, kernel_service, session_service);

    app_service.createDirectory("testdir", "/");

    auto dto = app_service.listDirectory("/");
    REQUIRE(dto.list.size() >= 1);
    REQUIRE(std::find(dto.list.begin(), dto.list.end(), "testdir") != dto.list.end());
}
