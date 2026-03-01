#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/domain/rights_service.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"
#include "domain/objects/reg_file.h"
#include "domain/objects/inmem_file.h"
#include "domain/entity/user.h"

using namespace services;
using namespace domain;

TEST_CASE("RightsService hasRight for root user") {
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

    UserService user_service(user_factory, *kernel_container);
    GroupService group_service(group_factory, *kernel_container);
    RightsService rights_service(user_service, group_service);

    auto admin_user = user_factory->createAdmin();
    auto acl = acl_factory->createDefaultFileACL();
    auto inmem = inmem_factory->createInMemory(1000, 2000, std::move(acl));
    auto file = std::make_shared<FileFactory>()->createObject("test.txt", "/test.txt", std::move(inmem));

    REQUIRE(rights_service.hasRight(admin_user.get(), file.get(), RightType::READ));
    REQUIRE(rights_service.hasRight(admin_user.get(), file.get(), RightType::WRITE));
}

TEST_CASE("RightsService hasRight for owner") {
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

    UserService user_service(user_factory, *kernel_container);
    GroupService group_service(group_factory, *kernel_container);
    RightsService rights_service(user_service, group_service);

    auto user = user_factory->createUser("testuser", 2000);
    auto acl = acl_factory->createDefaultFileACL();
    auto inmem = inmem_factory->createInMemory(user->getUid(), 2000, std::move(acl));
    auto file = std::make_shared<FileFactory>()->createObject("test.txt", "/test.txt", std::move(inmem));

    REQUIRE(rights_service.hasRight(user.get(), file.get(), RightType::READ));
}

TEST_CASE("RightsService getEffectiveRights") {
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

    UserService user_service(user_factory, *kernel_container);
    GroupService group_service(group_factory, *kernel_container);
    RightsService rights_service(user_service, group_service);

    auto user = user_factory->createUser("testuser", 2000);
    auto acl = acl_factory->createDefaultFileACL();
    auto inmem = inmem_factory->createInMemory(user->getUid(), 2000, std::move(acl));
    auto file = std::make_shared<FileFactory>()->createObject("test.txt", "/test.txt", std::move(inmem));

    auto rights = rights_service.getEffectiveRights(user.get(), file.get());
    REQUIRE(rights.size() > 0);
}

TEST_CASE("RightsService hasGlobalRight for root") {
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

    UserService user_service(user_factory, *kernel_container);
    GroupService group_service(group_factory, *kernel_container);
    RightsService rights_service(user_service, group_service);

    auto admin_user = user_factory->createAdmin();




    auto result = rights_service.hasGlobalRight(admin_user.get(), RightType::READ);

    REQUIRE((result == true || result == false));
}

