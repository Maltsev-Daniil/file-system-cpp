#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/application/app_file_service.h"
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
#include "dto/objects/file_content_dto.h"
#include "domain/objects/directory.h"
#include "domain/objects/directory_structure.h"

using namespace services;
using namespace domain;

TEST_CASE("FileAppService createFile") {
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

    SessionService session_service;
    session_service.login("admin", 0, "/");

    auto file_service = std::make_shared<FileService>(file_factory, inmem_factory, acl_factory);
    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    FileAppService app_service(rights_service, file_service, dir_service, kernel_service, session_service);

    app_service.createFile("test.txt", "/");

    auto* root = kernel_service->resolvePath("/");
    auto* dir = dynamic_cast<IDirectory*>(root);
    REQUIRE(dir != nullptr);
    REQUIRE(dir->getDirectoryStructure()->contains("test.txt"));
}

TEST_CASE("FileAppService writeFile and readFile") {
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

    SessionService session_service;
    session_service.login("admin", 0, "/");

    auto file_service = std::make_shared<FileService>(file_factory, inmem_factory, acl_factory);
    auto dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

    FileAppService app_service(rights_service, file_service, dir_service, kernel_service, session_service);

    app_service.createFile("test.txt", "/");

    std::string test_data = "Hello, World!";
    app_service.writeFile("/test.txt", test_data);

    auto dto = app_service.readFile("/test.txt");
    REQUIRE(dto.data == test_data);
}
