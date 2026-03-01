#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/application/app_user_service.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "services/domain/rights_service.h"
#include "services/domain/kernel_service.h"
#include "services/session/session_service.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"
#include "requests/user/req_useradd.h"
#include "requests/user/req_usermod.h"
#include "domain/entity/user.h"
#include "domain/entity/group.h"

using namespace services;
using namespace domain;

TEST_CASE("UserAppService createUser") {
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

    UserAppService app_service(
        rights_service,
        kernel_service,
        user_service,
        group_service,
        session_service
    );


    std::string username = "testuser";
    std::string groupname = "usergroup";
    app_service.createUser(username, groupname);

    auto user_table = kernel_container->getUserTable();
    REQUIRE(user_table->contains("testuser"));

    auto group_table = kernel_container->getGroupTable();
    REQUIRE(group_table->contains(groupname));
}

TEST_CASE("UserAppService userLogin and userLogout") {
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

    UserAppService app_service(
        rights_service,
        kernel_service,
        user_service,
        group_service,
        session_service
    );


    app_service.userLogin("admin");
    app_service.userLogout();
}

TEST_CASE("UserAppService userModify add secondary groups") {
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

    UserAppService app_service(
        rights_service,
        kernel_service,
        user_service,
        group_service,
        session_service
    );


    std::string username = "testuser";
    std::string group1 = "group1";
    std::string group2 = "group2";
    std::string group3 = "group3";
    app_service.createUser(username, group1);
    group_service->addGroupToTable(group_service->createGroup(group2));
    group_service->addGroupToTable(group_service->createGroup(group3));

    UsermodRequest req;
    req.kind = UsermodOpt::ADD_SECONDARY_GROUPS;
    req.groups = {group2, group3};

    app_service.userModify(username, req);

    auto* user = kernel_container->getUserTable()->getByName(username);
    REQUIRE(user->getSecondaryGroups().size() == 2);
}

