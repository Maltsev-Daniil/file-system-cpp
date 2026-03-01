//
// Created by nitsir on 1/4/26.
//


#include "factories/persistence/persistence_fac.h"

#include "containers/persistence_container.h"
#include "services/application/app_filesystem_persistance_service.h"

using namespace services;
PersistenceServiceFactory::PersistenceServiceFactory(SystemContainer &system_container,
    domain::IKernelFactory &kernel_factory, services::ISessionService &session_service,
    IPersistenceContainer &persistence_container)
        : system_container(system_container), kernel_factory(kernel_factory), session_service(session_service),
persistence_container(persistence_container) {}

std::unique_ptr<services::IFileSystemPersistenceAppService> PersistenceServiceFactory::
createFileSystemPersistenceService() {
    return std::make_unique<services::FileSystemPersistenceAppService>(
        system_container,
        *persistence_container.getFsMapper(),
        kernel_factory,
        *persistence_container.getSerializer(),
        session_service);
}

