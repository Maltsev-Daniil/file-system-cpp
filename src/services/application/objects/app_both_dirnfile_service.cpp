#include "services/application/app_both_dirnfile_service.h"

#include "../../../../debug.h"
#include "domain/entity/user.h"
#include "domain/objects/directory.h"
#include "domain/objects/file_object.h"
#include "services/domain/rights_service.h"
#include "services/domain/kernel_service.h"
#include "services/domain/file_service.h"
#include "services/domain/dir_service.h"

#include "services/session/session_service.h"
#include "../../../../include/dto/objects/stat_file_dto.h"

namespace services {

void DirNFileAppService::remove(const std::string &path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("remove: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    // просто пробрасываем исключение если что-то не так
    auto file_object = kernel->resolvePath(path);

    // getPath возвращает абсолютный путь к родительскому каталогу !!!! НЕВРНО
    auto dir_where_obj = kernel->resolveParentDir(*file_object);

    if (!rights->hasRight(user, dir_where_obj, domain::RightType::WRITE)) {
        throw std::runtime_error(
            "remove: access denied: no WRITE permission on parent directory"
        );
    }

    if (!rights->hasRight(user, dir_where_obj, domain::RightType::EXECUTE)) {
        throw std::runtime_error(
            "remove: access denied: no EXECUTE permission on parent directory"
        );
    } 

    kernel->remove(file_object, dir_where_obj);
    
    dirs->updateDirectorySizeRecursive(dir_where_obj, kernel.get());
    
    kernel->updateModTime(dir_where_obj);
}

StatFileDTO DirNFileAppService::getStat(const std::string & path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("remove: user is not logged in");
    }
    auto user = kernel->resolveUser(session.getCurrentUsername());

    auto obj = kernel->resolvePath(path);
    if (!rights->hasRight(user, obj, domain::RightType::READ))
        throw std::runtime_error("getStat: access denied");

    auto inmem = obj->getInMemoryFile();
    return StatFileDTO(inmem);
}

void DirNFileAppService::move(const std::string &path, const std::string &target_dir_path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("move: user is not logged in");
    }
    auto user = kernel->resolveUser(session.getCurrentUsername());

    auto obj = kernel->resolvePath(path);
    auto parent_dir_obj = kernel->resolveParentDir(*obj);
    auto target_obj = kernel->resolvePath(target_dir_path);
    domain::IDirectory* target_dir;
    if (!(target_dir = dynamic_cast<domain::IDirectory*>(target_obj)))
        throw std::runtime_error("move: target obj not a directory");

    if (!rights->hasRight(user, parent_dir_obj, domain::RightType::WRITE)) {
        throw std::runtime_error(
            "move: access denied: no WRITE permission on source directory"
        );
    }

    if (!rights->hasRight(user, parent_dir_obj, domain::RightType::EXECUTE)) {
        throw std::runtime_error(
            "move: access denied: no EXECUTE permission on source directory"
        );
    }

    if (!rights->hasRight(user, target_dir, domain::RightType::WRITE)) {
        throw std::runtime_error(
            "move: access denied: no WRITE permission on target directory"
        );
    }

    if (!rights->hasRight(user, target_dir, domain::RightType::EXECUTE)) {
        throw std::runtime_error(
            "move: access denied: no EXECUTE permission on target directory"
        );
    }

    kernel->move(obj, parent_dir_obj, target_dir);
    
    dirs->updateDirectorySizeRecursive(parent_dir_obj, kernel.get());

    dirs->updateDirectorySizeRecursive(target_dir, kernel.get());
    
    kernel->updateModTime(obj);
    kernel->updateModTime(parent_dir_obj);
    kernel->updateModTime(target_dir);
}


void DirNFileAppService::copy(const std::string &path,
                             const std::string &target_dir_path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("copy: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    auto src_obj = kernel->resolvePath(path);
    auto target_obj = kernel->resolvePath(target_dir_path);
    domain::IDirectory* target_dir;
    if (!(target_dir = dynamic_cast<domain::IDirectory*>(target_obj)))
        throw std::runtime_error("move: target obj not a directory");

    if (!rights->hasRight(user, src_obj, domain::RightType::READ)) {
        throw std::runtime_error(
            "copy: access denied: no READ permission on source object"
        );
    }

    if (!rights->hasRight(user, src_obj, domain::RightType::EXECUTE)) {
        throw std::runtime_error(
            "copy: access denied: no EXECUTE permission on source object"
        );
    }

    if (!rights->hasRight(user, target_dir, domain::RightType::WRITE)) {
        throw std::runtime_error(
            "copy: access denied: no WRITE permission on target directory"
        );
    }

    if (!rights->hasRight(user, target_dir, domain::RightType::EXECUTE)) {
        throw std::runtime_error(
            "copy: access denied: no EXECUTE permission on target directory"
        );
    }

    // TODO надо поменять тут сигнатуру чтобы мы внутри потом обновили пути
    auto copy_obj = kernel->copy(src_obj,user->getUid(), user->getGroup());
    copy_obj->updatePathRecursive(target_dir->getPath());
    dirs->insertEntry(std::move(copy_obj), target_dir);
    
    dirs->updateDirectorySizeRecursive(target_dir, kernel.get());
    
    kernel->updateModTime(target_dir);
}
}