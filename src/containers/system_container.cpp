
#include "containers/system_container.h"

#include "containers/persistence_container.h"
#include "factories/init/app_services_fac.h"
#include "factories/init/cmdlet_fac.h"
#include "factories/init/domain_services_fac.h"
#include "factories/persistence/persistence_fac.h"

#include "factories/init/kernel_fac.h"
#include "mappers/acl/acl_mapper.h"
#include "mappers/objects/metadata_mapper.h"
#include "mappers/objects/regfile_mapper.h"
#include "mappers/objects/file_object_mapper.h"
#include "mappers/objects/adapters/ifo_adapter_mapper.h"
#include "mappers/entity/user_mapper.h"
#include "mappers/entity/group_mapper.h"
#include "mappers/file_system/file_system_mapper.h"

#include "services/application/app_filesystem_persistance_service.h"

using namespace services;

SystemContainer::SystemContainer(
    std::shared_ptr<domain::IKernelContainer> kernel_container,
    std::unique_ptr<services::IDomainServicesFactory> domain_factory,
    std::shared_ptr<ICmdDispatcher> dispatcher,
    IOutput &output_,
    std::unique_ptr<domain::IKernelFactory> kernel_factory,
    std::shared_ptr<domain::IACLFactory> acl_factory,
    std::shared_ptr<domain::IInMemoryFactory> inmem_factory
)
    : kernel_container(std::move(kernel_container))
      , domain_factory(std::move(domain_factory))
      , dispatcher(std::move(dispatcher))
      , output(output_)
    , kernel_factory(std::move(kernel_factory))
    , acl_factory(std::move(acl_factory))
    , inmem_factory(std::move(inmem_factory)) {}

IKernelService *SystemContainer::getKernelService() const {
    return kernel_service.get();
}

IFileService *SystemContainer::getFileService() const {
    return file_service.get();
}

IDirectoryService *SystemContainer::getDirectoryService() const {
    return dir_service.get();
}

IUserService *SystemContainer::getUserService() const {
    return user_service.get();
}

IGroupService *SystemContainer::getGroupService() const {
    return group_service.get();
}

IRightsService *SystemContainer::getRightsService() const {
    return rights_service.get();
}


IFileAppService *SystemContainer::getFileAppService() const {
    return file_app.get();
}

IDirectoryAppService *SystemContainer::getDirectoryAppService() const {
    return dir_app.get();
}

IDirNFileAppService *SystemContainer::getDirNFileAppService() const {
    return dir_n_file_app.get();
}

IFileSystemAppService *SystemContainer::getFileSystemAppService() const {
    return fs_app.get();
}

IFileSystemPersistenceAppService *SystemContainer::getFileSystemPersistenceAppService() const {
    return fs_persistence_app.get();
}

IUserAppService *SystemContainer::getUserAppService() const {
    return user_app.get();
}

IGroupAppService *SystemContainer::getGroupAppService() const {
    return group_app.get();
}

IRightsAppService *SystemContainer::getRightsAppService() const {
    return rights_app.get();
}

void SystemContainer::cleanupServices() {
    if (dispatcher) {
        dispatcher->clearCmdlets();
    }
    
    cmdlet_factory.reset();
    
    fs_persistence_app.reset();
    rights_app.reset();
    group_app.reset();
    user_app.reset();
    fs_app.reset();
    dir_n_file_app.reset();
    dir_app.reset();
    file_app.reset();
    
    persistence_factory.reset();
    app_factory.reset();
    path_resolver.reset();
}

void SystemContainer::initServices() {
    kernel_service = domain_factory->createKernelService();
    user_service = domain_factory->createUserService();
    group_service = domain_factory->createGroupService();
    rights_service = domain_factory->createRightsService(
        user_service,
        group_service
    );
    
    path_resolver = std::make_shared<services::PathPatternResolver>(
        *kernel_service,
        *session_service);
    
    app_factory = std::make_unique<services::AppFactory>(
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
    fs_app = app_factory->createFileSystemAppService();
    user_app = app_factory->createUserAppService();
    group_app = app_factory->createGroupAppService();
    rights_app = app_factory->createRightsAppService();
    
    persistence_factory = std::make_unique<PersistenceServiceFactory>(
        *this,
        *kernel_factory,
        *session_service,
        *persistence_container);
    
    fs_persistence_app = persistence_factory->createFileSystemPersistenceService();
    
    cmdlet_factory = std::make_unique<CmdletFactory>(
        file_app,
        dir_app,
        rights_app,
        dir_n_file_app,
        fs_app,
        fs_persistence_app,
        user_app,
        group_app,
        path_resolver,
        dispatcher,
        output
    );
    
    cmdlet_factory->registerCmdlets();
}

void SystemContainer::replaceKernelContainer(
    std::shared_ptr<domain::IKernelContainer> new_cont) {
    if (!new_cont) {
        throw std::invalid_argument("SystemContainer::replaceKernelContainer: new container is null");
    }
    
    cleanupServices();
    
    kernel_container = std::move(new_cont);
    
    auto* domain_fac = dynamic_cast<DomainServicesFactory*>(domain_factory.get());
    if (domain_fac) {
        domain_fac->updateKernelContainer(kernel_container);
    }
    
    initServices();
}

void SystemContainer::init() {
    // эти методы не зависят от kernel_container
    file_service = domain_factory->createFileService();
    dir_service = domain_factory->createDirectoryService();
    session_service = domain_factory->createSessionService();
    
    persistence_container = std::make_unique<PersistenceContainer>(
        *acl_factory,
        *inmem_factory);

    // эти сервисы зависят от kernel_container
    initServices();
}
