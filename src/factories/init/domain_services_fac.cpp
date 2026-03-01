#include "factories/init/domain_services_fac.h"

#include "services/domain/file_service.h"
#include "services/domain/dir_service.h"
#include "../../../include/services/application/app_filesystem_persistance_service.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "services/domain/rights_service.h"
#include "services/domain/kernel_service.h"
#include "services/session/session_service.h"

namespace services {

DomainServicesFactory::DomainServicesFactory(
    std::shared_ptr<domain::IFileObjectFactory> file_factory,
    std::shared_ptr<domain::IFileObjectFactory> dir_factory,
    std::shared_ptr<domain::IUserFactory> user_factory,
    std::shared_ptr<domain::IGroupFactory> group_factory,
    std::shared_ptr<domain::IInMemoryFactory> inmem_factory,
    std::shared_ptr<domain::IACLFactory> acl_factory,
    std::shared_ptr<domain::IKernelContainer> kernel_container
)
    : file_factory(file_factory),
      dir_factory(dir_factory),
      user_factory(user_factory),
      group_factory(group_factory),
      inmem_factory(inmem_factory),
      acl_factory(acl_factory),
      kernel_container(kernel_container)
{
}

std::shared_ptr<IFileService> DomainServicesFactory::createFileService()
{
    return std::make_shared<FileService>(
        file_factory,
        inmem_factory,
        acl_factory
    );
}

std::shared_ptr<IDirectoryService> DomainServicesFactory::createDirectoryService()
{
    return std::make_shared<DirectoryService>(
        dir_factory,
        inmem_factory,
        acl_factory
    );
}

std::shared_ptr<IUserService> DomainServicesFactory::createUserService()
{
    return std::make_shared<UserService>(
        user_factory,
        *kernel_container
    );
}

std::shared_ptr<IGroupService> DomainServicesFactory::createGroupService()
{
    return std::make_shared<GroupService>(
        group_factory,
        *kernel_container
    );
}

std::shared_ptr<IRightsService> DomainServicesFactory::createRightsService(
    std::shared_ptr<IUserService> user_service,
    std::shared_ptr<IGroupService> group_service
)
{
    return std::make_shared<RightsService>(
        *user_service,
        *group_service
    );
}

std::shared_ptr<IKernelService> DomainServicesFactory::createKernelService()
{
    return std::make_shared<KernelService>(
        kernel_container,
        inmem_factory
    );
}

std::shared_ptr<ISessionService> DomainServicesFactory::createSessionService() {
    return std::make_shared<SessionService>();
}

void DomainServicesFactory::updateKernelContainer(
    std::shared_ptr<domain::IKernelContainer> new_container)
{
    if (!new_container) {
        throw std::invalid_argument("DomainServicesFactory::updateKernelContainer: new_container is null");
    }
    kernel_container = std::move(new_container);
}

} // namespace services