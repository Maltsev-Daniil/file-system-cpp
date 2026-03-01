#include "services/domain/rights_service.h"
#include "services/application/app_file_service.h"

#include "../../../../debug.h"
#include "domain/entity/user.h"
#include "domain/objects/directory.h"
#include "domain/objects/reg_file.h"
#include "../../../../include/dto/objects/file_content_dto.h"
#include "services/domain/file_service.h"
#include "services/domain/kernel_service.h"
#include "services/session/session_service.h"
#include "services/domain/dir_service.h"

namespace services {

FileContentDTO FileAppService::readFile(const std::string& path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("readFile: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    // просто пробрасываем исключение если что-то не так
    auto file_object = kernel->resolvePath(path);

    if (!dynamic_cast<domain::IRegularFile*>(file_object))
        throw std::runtime_error("readFile: not a regular file");

    if (!rights->hasRight(user, file_object, domain::RightType::READ)) {
        throw std::runtime_error("readFile: access denied");
    }

    const std::string& content = files->readFile(file_object);

    FileContentDTO dto;
    dto.data = std::move(content);

    return dto;
}

void FileAppService::writeFile(const std::string &path, const std::string &data)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("writeFile: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    // просто пробрасываем исключение если что-то не так
    auto file_object = kernel->resolvePath(path);

    if (!dynamic_cast<domain::IRegularFile*>(file_object))
        throw std::runtime_error("writeFile: not a regular file");

    if (!rights->hasRight(user, file_object, domain::RightType::WRITE)) {
        throw std::runtime_error("writeFile: access denied");
    }

    files->openFileForWrite(file_object);

    files->writeFile(file_object, data);

    domain::IDirectory* parent = kernel->resolveParentDir(*file_object);
    if (parent) {
        dirs->updateDirectorySizeRecursive(parent, kernel.get());
    }

    kernel->updateModTime(file_object);

    files->closeFile(file_object);
}

void FileAppService::appendFile(const std::string& path, const std::string& data)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("appendFile: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    auto file_object = kernel->resolvePath(path);

    if (!dynamic_cast<domain::IRegularFile*>(file_object)) {
        throw std::runtime_error("appendFile: not a regular file");
    }

    if (!rights->hasRight(user, file_object, domain::RightType::WRITE)) {
        throw std::runtime_error("appendFile: access denied");
    }

    files->openFileForWrite(file_object);

    files->appendToFile(file_object, data);

    domain::IDirectory* parent = kernel->resolveParentDir(*file_object);
    if (parent) {
        dirs->updateDirectorySizeRecursive(parent, kernel.get());
    }

    kernel->updateModTime(file_object);

    files->closeFile(file_object);
}

void FileAppService::createFile(const std::string& filename, const std::string &dir_path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("createFile: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    auto file_obj = kernel->resolvePath(dir_path);
    auto dir = dynamic_cast<domain::IDirectory*>(file_obj);
    if (!dir)
        throw std::runtime_error("createFile: invalid path");

    if (!rights->hasRight(user, dir, domain::RightType::WRITE)) {
        throw std::runtime_error("createFile: access denied");
    }

    auto new_file = files->createFile(dir_path, filename, user->getUid(), user->getGroup());
    DEBUG_LOG(new_file->getPath());
    dirs->insertEntry(std::move(new_file), dir);
    
    kernel->updateModTime(dir);
}

}