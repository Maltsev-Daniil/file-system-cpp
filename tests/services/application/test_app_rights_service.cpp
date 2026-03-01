#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/application/app_rights_service.h"
#include "services/application/app_file_service.h"
#include "services/application/app_dir_service.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "services/domain/rights_service.h"
#include "services/domain/kernel_service.h"
#include "services/domain/file_service.h"
#include "services/domain/dir_service.h"
#include "services/session/session_service.h"
#include "domain/objects/file_object.h"
#include "domain/objects/inmem_file.h"
#include "domain/entity/user.h"
#include "domain/entity/group.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"
#include "domain/objects/acl.h"
#include "domain/types.h"
#include "dto/rights/file_acl_dto.h"
#include "dto/rights/e_rights_dto.h"

using namespace services;
using namespace domain;

TEST_CASE("RightsAppService changeMod owner user") {
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

    RightsAppService app_service(rights_service, kernel_service, session_service);


    auto file_app_service = std::make_shared<FileAppService>(
        rights_service,
        std::make_shared<FileService>(file_factory, inmem_factory, acl_factory),
        std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory),
        kernel_service,
        session_service
    );
    auto dir_app_service = std::make_shared<DirectoryAppService>(
        rights_service,
        std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory),
        kernel_service,
        session_service
    );
    dir_app_service->createDirectory("testdir", "/");
    file_app_service->createFile("test.txt", "/testdir");

    std::set<ACLItem> mask = {ACLItem(RightType::READ, AccessType::ALLOW)};
    app_service.changeMod("/testdir/test.txt", ChmodTargetKind::OWNER_USER, "", mask);

    auto* file = kernel_service->resolvePath("/testdir/test.txt");
    auto acl = file->getInMemoryFile()->getAcl();
    REQUIRE(acl->getOwnerUserMask().size() == 1);
}

TEST_CASE("RightsAppService changeMod without login") {
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


    RightsAppService app_service(rights_service, kernel_service, session_service);

    std::set<ACLItem> mask = {ACLItem(RightType::READ, AccessType::ALLOW)};
    REQUIRE_THROWS_AS(
        app_service.changeMod("/", ChmodTargetKind::OWNER_USER, "", mask),
        std::runtime_error
    );
}

TEST_CASE("RightsAppService changeOwn") {
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

    RightsAppService app_service(rights_service, kernel_service, session_service);


    std::string username = "testuser";
    std::string groupname = "testgroup";
    auto group = group_service->createGroup(groupname);
    auto gid = group->getGid();
    group_service->addGroupToTable(std::move(group));
    auto user = user_service->createUser(username, gid);
    user_service->addUserToTable(std::move(user));
    auto* user_ptr = kernel_container->getUserTable()->getByName(username);

    auto file_app_service = std::make_shared<FileAppService>(
        rights_service,
        std::make_shared<FileService>(file_factory, inmem_factory, acl_factory),
        std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory),
        kernel_service,
        session_service
    );
    file_app_service->createFile("test.txt", "/");

    app_service.changeOwn("/test.txt", username);

    auto* file = kernel_service->resolvePath("/test.txt");
    REQUIRE(file->getInMemoryFile()->getUid() == user_ptr->getUid());
}

TEST_CASE("RightsAppService changeGrp") {
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

    RightsAppService app_service(rights_service, kernel_service, session_service);


    std::string groupname = "testgroup";
    auto group = group_service->createGroup(groupname);
    group_service->addGroupToTable(std::move(group));
    auto* group_ptr = kernel_container->getGroupTable()->getByName(groupname);

    auto file_app_service = std::make_shared<FileAppService>(
        rights_service,
        std::make_shared<FileService>(file_factory, inmem_factory, acl_factory),
        std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory),
        kernel_service,
        session_service
    );
    file_app_service->createFile("test.txt", "/");

    app_service.changeGrp("/test.txt", groupname);

    auto* file = kernel_service->resolvePath("/test.txt");
    REQUIRE(file->getInMemoryFile()->getGid() == group_ptr->getGid());
}

TEST_CASE("RightsAppService getFileAcl") {
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

    RightsAppService app_service(rights_service, kernel_service, session_service);

    auto file_app_service = std::make_shared<FileAppService>(
        rights_service,
        std::make_shared<FileService>(file_factory, inmem_factory, acl_factory),
        std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory),
        kernel_service,
        session_service
    );
    file_app_service->createFile("test.txt", "/");

    auto dto = app_service.getFileAcl("/test.txt");
    REQUIRE(dto.file_path == "/test.txt");
    REQUIRE(dto.owner == "admin");
}

TEST_CASE("RightsAppService getFileERights") {
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

    RightsAppService app_service(rights_service, kernel_service, session_service);

    auto file_app_service = std::make_shared<FileAppService>(
        rights_service,
        std::make_shared<FileService>(file_factory, inmem_factory, acl_factory),
        std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory),
        kernel_service,
        session_service
    );
    file_app_service->createFile("test.txt", "/");

    auto dto = app_service.getFileERights("/test.txt", "");

    REQUIRE(dto.e_rights.size() >= 0);
}

