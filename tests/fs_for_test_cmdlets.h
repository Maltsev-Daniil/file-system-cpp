#pragma once

#include <memory>
#include <string>

#include "domain/types.h"
#include "containers/kernel_container.h"

#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "factories/init/kernel_fac.h"
#include "factories/init/domain_services_fac.h"
#include "factories/init/app_services_fac.h"

#include "services/domain/kernel_service.h"
#include "services/domain/file_service.h"
#include "services/domain/dir_service.h"
#include "services/session/session_service.h"
#include "services/shell/path_pattern_resolver.h"

#include "services/application/app_file_service.h"
#include "services/application/app_dir_service.h"
#include "services/application/app_both_dirnfile_service.h"

#include "cmd_dispatcher.h"
#include "io.h"

namespace cmdlets {
class ICmdlet;
}

namespace services {
class IFileAppService;
class IDirectoryAppService;
class IDirNFileAppService;
class IPathPatternResolver;
}

class ICmdDispatcher;

class FsForTestCmdlets {
public:
    FsForTestCmdlets();
    ~FsForTestCmdlets() = default;

    domain::InodeGenerator inode_gen;
    domain::UidGenerator uid_gen;
    domain::GidGenerator gid_gen;

    std::shared_ptr<domain::InMemoryFactory> inmem_factory;
    std::shared_ptr<domain::FileFactory> file_factory;
    std::shared_ptr<domain::DirectoryFactory> dir_factory;
    std::shared_ptr<domain::UserFactory> user_factory;
    std::shared_ptr<domain::GroupFactory> group_factory;
    std::shared_ptr<domain::ACLFactory> acl_factory;

    std::unique_ptr<domain::KernelFactory> kernel_factory;
    std::shared_ptr<domain::IKernelContainer> kernel_container;

    std::unique_ptr<services::DomainServicesFactory> domain_services_factory;

    std::shared_ptr<services::IKernelService> kernel_service;
    std::shared_ptr<services::IFileService> file_service;
    std::shared_ptr<services::IDirectoryService> dir_service;
    std::shared_ptr<services::ISessionService> session_service;

    std::unique_ptr<services::AppFactory> app_factory;

    std::shared_ptr<services::IFileAppService> file_app;
    std::shared_ptr<services::IDirectoryAppService> dir_app;
    std::shared_ptr<services::IDirNFileAppService> dir_n_file_app;

    std::shared_ptr<services::IPathPatternResolver> path_resolver;

    std::shared_ptr<ICmdDispatcher> dispatcher;
    std::shared_ptr<IOutput> output;

    void initialize();

    void login(const std::string& username, domain::userId_t uid, const std::string& home_dir = "/");

    ICmdDispatcher* getDispatcher() const { return dispatcher.get(); }

    services::IKernelService* getKernelService() const { return kernel_service.get(); }

    services::ISessionService* getSessionService() const { return session_service.get(); }

    void registerBasicCmdlets();

private:
    void createFactories();
    void createKernel();
    void createDomainServices();
    void createAppServices();
};
