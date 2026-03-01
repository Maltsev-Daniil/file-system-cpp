#include "domain/types.h"
#include "domain/objects/file_object.h"
#include "services/domain/file_service.h"
#include "domain/objects/inmem_file.h"
#include "domain/tables/user_table.h"
#include "containers/kernel_container.h"
#include "../../../../include/domain/objects/directory_structure.h"
#include "../../../../include/factories/objects/acl_fac.h"
#include "../../../../include/factories/objects/inmem_fac.h"
#include <cassert>
#include <memory>

#include "domain/objects/reg_file.h"
#include "../../../../include/factories/objects/file_object_fac.h"

using namespace domain;

namespace services {

FileService::FileService(std::shared_ptr<IFileObjectFactory> ff,
                         std::shared_ptr<IInMemoryFactory> imf,
                         std::shared_ptr<IACLFactory> af)
    : file_fac(std::move(ff))
    , inmem_fac(std::move(imf))
    , acl_fac(std::move(af))
{
    if (!file_fac || !inmem_fac || !acl_fac)
        throw std::invalid_argument("FileService: factories must not be null");
}

std::unique_ptr<IFileObject> FileService::createFile(
        const std::string& dir_path,
        const std::string& file_name,
        userId_t owner_uid,
        groupId_t owner_gid)
{
    assert(!dir_path.empty());
    assert(!file_name.empty());
    assert(dir_path[0] == '/');

    auto acl = acl_fac->createDefaultFileACL();

    auto inmem = inmem_fac->createInMemory(owner_uid, owner_gid, std::move(acl));
    if (!inmem)
        throw std::runtime_error("FileService: cannot create InMemoryFile");

    std::string full_path = dir_path;
    if (full_path.back() != '/')
        full_path += "/";
    full_path += file_name;

    auto file = file_fac->createObject(file_name, full_path, std::move(inmem));
    if (!file)
        throw std::runtime_error("FileService: cannot create FileObject");

    return file;
}

void FileService::openFileForRead(IFileObject* file) {
    if (!file) throw std::invalid_argument("openFileForRead: file is null");

    auto* reg = dynamic_cast<IRegularFile*>(file);
    if (!reg) throw std::runtime_error("openFileForRead: not a regular file");

    reg->setLock(LockType::READ);
}

void FileService::openFileForWrite(IFileObject* file) {
    if (!file) throw std::invalid_argument("openFileForWrite: file is null");

    auto* reg = dynamic_cast<IRegularFile*>(file);
    if (!reg) throw std::runtime_error("openFileForWrite: not a regular file");

    reg->setLock(LockType::WRITE);
}

void FileService::closeFile(IFileObject* file) {
    if (!file) throw std::invalid_argument("closeFile: file is null");

    auto* reg = dynamic_cast<IRegularFile*>(file);
    if (!reg) throw std::runtime_error("closeFile: not a regular file");

    reg->setLock(LockType::NONE);
}

const std::string& FileService::readFile(IFileObject* file) {
    if (!file) throw std::invalid_argument("readFile: file is null");

    auto* reg = dynamic_cast<IRegularFile*>(file);
    if (!reg) throw std::runtime_error("readFile: not a regular file");

    return reg->getContent();
}

void FileService::writeFile(IFileObject* file, const std::string& data) {
    if (!file) throw std::invalid_argument("writeFile: file is null");

    auto* reg = dynamic_cast<IRegularFile*>(file);
    if (!reg) throw std::runtime_error("writeFile: not a regular file");

    reg->setContent(data);
    reg->getInMemoryFile()->setSize(data.size());
    reg->getInMemoryFile()->updateModificationTime();
}

void FileService::appendToFile(IFileObject* file, const std::string& data) {
    if (!file) throw std::invalid_argument("appendToFile: file is null");

    auto* reg = dynamic_cast<IRegularFile*>(file);
    if (!reg) throw std::runtime_error("appendToFile: not a regular file");

    std::string new_content = reg->getContent() + data;
    reg->setContent(new_content);
    reg->getInMemoryFile()->setSize(new_content.size());
    reg->getInMemoryFile()->updateModificationTime();
}

IInMemoryFile* FileService::getFileInfo(IFileObject* file) {
    if (!file) throw std::invalid_argument("getFileInfo: file is null");

    auto* reg = dynamic_cast<IRegularFile*>(file);
    if (!reg) throw std::runtime_error("getFileInfo: not a regular file");

    return reg->getInMemoryFile();
}

size_t FileService::getSize(IFileObject* file) const {
    if (!file) throw std::invalid_argument("getSize: file is null");

    auto* reg = dynamic_cast<IRegularFile*>(file);
    if (!reg) throw std::runtime_error("getSize: not a regular file");

    return reg->getContent().size();
}

void FileService::renameFile(IFileObject* file, const std::string& new_name) {
    if (!file) throw std::invalid_argument("renameFile: file is null");
    if (new_name.empty()) throw std::invalid_argument("renameFile: name empty");

    file->setName(new_name);
}

// TODO не забыть поменять path ------- удалить
std::unique_ptr<IFileObject> FileService::copyFile(IFileObject* file,
                            userId_t uid, groupId_t gid) {
    std::unique_ptr<IFileObject> file_clone = file->clone();
    inmem_fac->generateId(file_clone->getInMemoryFile());
    file_clone->getInMemoryFile()->setUid(uid);
    file_clone->getInMemoryFile()->setGid(gid);
    return file_clone;
}

// TODO удалить и из хедера
void FileService::moveFile(IFileObject* file, const std::string& target_dir) {
    throw std::runtime_error("moveFile: not implemented");
}

} // namespace services