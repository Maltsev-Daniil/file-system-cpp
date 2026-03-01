#include "factories/init/app_services_fac.h"

#include "services/application/app_file_service.h"
#include "services/application/app_dir_service.h"
#include "services/application/app_both_dirnfile_service.h"
#include "services/application/app_filesystem_persistance_service.h"
#include "services/application/app_filesystem_service.h"
#include "services/application/app_user_service.h"
#include "services/application/app_group_service.h"
#include "services/application/app_rights_service.h"

namespace services {

AppFactory::AppFactory(
    std::shared_ptr<IKernelService> kernel,
    std::shared_ptr<IRightsService> rights,
    std::shared_ptr<IFileService> files,
    std::shared_ptr<IDirectoryService> dirs,
    std::shared_ptr<IUserService> users,
    std::shared_ptr<IGroupService> groups,
    ISessionService& session
)
    : kernel(std::move(kernel)),
      rights(std::move(rights)),
      files(std::move(files)),
      dirs(std::move(dirs)),
      users(std::move(users)),
      groups(std::move(groups)),
      session(session)
{
}

std::shared_ptr<IFileAppService> AppFactory::createFileAppService()
{
    return std::make_shared<FileAppService>(
        rights,
        files,
        dirs,
        kernel,
        session
    );
}

std::shared_ptr<IDirectoryAppService> AppFactory::createDirectoryAppService()
{
    return std::make_shared<DirectoryAppService>(
        rights,
        dirs,
        kernel,
        session
    );
}

std::shared_ptr<IDirNFileAppService> AppFactory::createDirNFileAppService()
{
    return std::make_shared<DirNFileAppService>(
        rights,
        dirs,
        kernel,
        session
    );
}

std::shared_ptr<IFileSystemAppService> AppFactory::createFileSystemAppService()
{
    return std::make_shared<FileSystemAppService>(
    kernel,
    session
    );
}

std::shared_ptr<IUserAppService> AppFactory::createUserAppService()
{
    return std::make_shared<UserAppService>(
        rights,
        kernel,
        users,
        groups,
        session
    );
}

std::shared_ptr<IGroupAppService> AppFactory::createGroupAppService()
{
    return std::make_shared<GroupAppService>(
        groups,
        users,
        kernel,
        rights,
        session
    );
}

std::shared_ptr<IRightsAppService> AppFactory::createRightsAppService()
{
    return std::make_shared<RightsAppService>(
        rights,
        kernel,
        session
    );
}

} // namespace services