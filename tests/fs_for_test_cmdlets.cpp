#include "fs_for_test_cmdlets.h"

#include "cmdlets/file/cat.h"
#include "cmdlets/file/touch.h"
#include "cmdlets/file/write.h"
#include "cmdlets/dir/ls.h"
#include "cmdlets/dir/mkdir.h"
#include "cmdlets/dir_n_file/cp.h"
#include "cmdlets/dir_n_file/mv.h"
#include "cmdlets/dir_n_file/rm.h"
#include "cmdlets/dir_n_file/stat.h"

using namespace services;
using namespace domain;
using namespace cmdlets;

FsForTestCmdlets::FsForTestCmdlets() {
    createFactories();
    createKernel();
    createDomainServices();
    createAppServices();

    dispatcher = std::make_shared<CmdDispatcher>();
    output = std::make_shared<ConsoleOutput>();
}

void FsForTestCmdlets::createFactories() {
    inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
    file_factory = std::make_shared<FileFactory>();
    dir_factory = std::make_shared<DirectoryFactory>();
    user_factory = std::make_shared<UserFactory>(uid_gen);
    group_factory = std::make_shared<GroupFactory>(gid_gen);
    acl_factory = std::make_shared<ACLFactory>();
}

void FsForTestCmdlets::createKernel() {
    kernel_factory = std::make_unique<KernelFactory>(
        group_factory,
        inmem_factory,
        user_factory
    );

    kernel_container = kernel_factory->createKernelContainerColdStart(
        user_factory,
        group_factory,
        dir_factory,
        inmem_factory,
        acl_factory
    );
}

void FsForTestCmdlets::createDomainServices() {
    domain_services_factory = std::make_unique<DomainServicesFactory>(
        file_factory,
        dir_factory,
        user_factory,
        group_factory,
        inmem_factory,
        acl_factory,
        kernel_container
    );

    kernel_service = domain_services_factory->createKernelService();
    file_service = domain_services_factory->createFileService();
    dir_service = domain_services_factory->createDirectoryService();
    session_service = domain_services_factory->createSessionService();
}

void FsForTestCmdlets::createAppServices() {
    auto user_service = domain_services_factory->createUserService();
    auto group_service = domain_services_factory->createGroupService();
    auto rights_service = domain_services_factory->createRightsService(
        user_service,
        group_service
    );

    app_factory = std::make_unique<AppFactory>(
        kernel_service,
        rights_service,
        file_service,
        dir_service,
        user_service,
        group_service,
        *session_service
    );

    file_app = app_factory->createFileAppService();
    dir_app = app_factory->createDirectoryAppService();
    dir_n_file_app = app_factory->createDirNFileAppService();

    path_resolver = std::make_shared<PathPatternResolver>(
        *kernel_service,
        *session_service
    );
}

void FsForTestCmdlets::initialize() {
    login("admin", 0, "/");
}

void FsForTestCmdlets::login(const std::string& username, domain::userId_t uid, const std::string& home_dir) {
    session_service->login(username, uid, home_dir);
}

void FsForTestCmdlets::registerBasicCmdlets() {
    dispatcher->registerCmdlet(
        "touch",
        std::make_unique<TouchCmdlet>(*file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "cat",
        std::make_unique<CatCmdlet>(*file_app, *path_resolver, *output)
    );

    dispatcher->registerCmdlet(
        "write",
        std::make_unique<WriteCmdlet>(*file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "mkdir",
        std::make_unique<MkdirCmdlet>(*dir_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "ls",
        std::make_unique<LsCmdlet>(*dir_app, *path_resolver, *output)
    );

    dispatcher->registerCmdlet(
        "cp",
        std::make_unique<CpCmdlet>(*dir_n_file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "mv",
        std::make_unique<MvCmdlet>(*dir_n_file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "rm",
        std::make_unique<RmCmdlet>(*dir_n_file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "stat",
        std::make_unique<StatCmdlet>(*dir_n_file_app, *path_resolver, *output)
    );
}
