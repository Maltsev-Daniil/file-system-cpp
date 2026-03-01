#include "services/application/app_dir_service.h"

#include "domain/entity/user.h"
#include "domain/objects/directory.h"
#include "services/domain/kernel_service.h"
#include "services/domain/rights_service.h"
#include "services/domain/dir_service.h"
#include "../../../../include/dto/objects/dir_content_dto.h"
#include "services/session/session_service.h"

void services::DirectoryAppService::createDirectory(const std::string &dirname, const std::string &dir_path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("createDirectory: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    auto file_obj = kernel->resolvePath(dir_path);
    auto dir = dynamic_cast<domain::IDirectory*>(file_obj);
    if (!dir)
        throw std::runtime_error("createDirectory: invalid path");

    if (!rights->hasRight(user, dir, domain::RightType::WRITE)) {
        throw std::runtime_error("createDirectory: access denied");
    }

    auto new_dir = dirs->createDirectory(dirname, dir_path, user->getUid(), user->getGroup());
    dirs->insertEntry(std::move(new_dir), dir);
    
    kernel->updateModTime(dir);
}

DirContentDTO services::DirectoryAppService::listDirectory(const std::string &path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("listDirectory: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    // просто пробрасываем исключение если что-то не так
    auto file_object = kernel->resolvePath(path);
    if (!dynamic_cast<domain::IDirectory*>(file_object))
        throw std::runtime_error("listDirectory: not a directory");

    if (!rights->hasRight(user, file_object, domain::RightType::READ)) {
        throw std::runtime_error("listDirectory: access denied");
    }

    std::vector<std::string> content = dirs->listDirectory(file_object);

    DirContentDTO dto;
    dto.list = std::move(content);

    return dto;
}
