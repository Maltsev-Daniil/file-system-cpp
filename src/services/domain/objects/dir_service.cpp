#include "domain/types.h"
#include "domain/objects/file_object.h"
#include "../../../../include/factories/objects/inmem_fac.h"
#include "../../../../include/factories/objects/file_object_fac.h"

#include "services/domain/dir_service.h"
#include "../../../../include/domain/objects/directory_structure.h"
#include "../../../../include/factories/objects/acl_fac.h"
#include "services/domain/file_service.h"
#include <cassert>
#include <memory>
#include <sstream>

#include "domain/objects/directory.h"
#include "domain/objects/inmem_file.h"
#include "services/domain/kernel_service.h"

#include "../../../../include/factories/objects/inmem_fac.h"

using namespace domain;

namespace services {

DirectoryService::DirectoryService(
    std::shared_ptr<IFileObjectFactory> df,
    std::shared_ptr<IInMemoryFactory> imf,
    std::shared_ptr<IACLFactory> af)
    : dir_fac(std::move(df))
    , inmem_fac(std::move(imf))
    , acl_fac(std::move(af))
{
    if (!dir_fac || !inmem_fac || !acl_fac)
        throw std::invalid_argument("DirectoryService: factories must not be null");
}

std::unique_ptr<IFileObject> DirectoryService::createDirectory(
    const std::string& name,
    const std::string& dir_path,
    userId_t owner_uid,
    groupId_t owner_gid)
{
    assert(!name.empty());
    assert(!dir_path.empty());
    assert(dir_path[0] == '/');

    auto acl = acl_fac->createDefaultFileACL();

    auto inmem = inmem_fac->createInMemory(owner_uid, owner_gid, std::move(acl));
    if (!inmem)
        throw std::runtime_error("DirectoryService: cannot create InMemoryFile for directory");
    std::string full_path = dir_path;
    if (full_path.back() != '/')
        full_path += "/";
    full_path += name;

    auto dir = dir_fac->createObject(name, full_path, std::move(inmem));
    if (!dir)
        throw std::runtime_error("DirectoryService: cannot create Directory");

    return dir;
}

void DirectoryService::renameDirectory(IFileObject* dir, const std::string& new_name) {
    if (!dir) throw std::invalid_argument("renameDirectory: dir is null");
    if (new_name.empty()) throw std::invalid_argument("renameDirectory: new name empty");

    dir->setName(new_name);
}

std::vector<std::string> DirectoryService::listDirectory(IFileObject* dir) {
    if (!dir) throw std::invalid_argument("listDirectory: dir is null");

    auto* directory = dynamic_cast<IDirectory*>(dir);
    if (!directory) throw std::runtime_error("listDirectory: object is not a directory");

    return directory->getDirectoryStructure()->listNames();
}

void DirectoryService::insertEntry(std::unique_ptr<IFileObject> object, IFileObject* dir_object) {
    std::string name = object->getName();

    if (name.empty())
        throw std::invalid_argument("insertEntry: object has empty name");

    auto dir = dynamic_cast<IDirectory*>(dir_object);
    if (!dir)
        throw std::invalid_argument("insertEntry: not a directory");

    if (!dir->getDirectoryStructure()->addEntry(std::move(object)))
        throw std::invalid_argument("insertEntry: object with such name already exist");
}

void DirectoryService::removeEntry(const std::string& name, IFileObject* dir_object) {
    if (name.empty()) throw std::invalid_argument("removeEntry: name is empty");

    auto dir = dynamic_cast<IDirectory*>(dir_object);
    if (!dir)
        throw std::invalid_argument("removeEntry: not a directory");

    if (!dir->getDirectoryStructure()->removeEntry(name))
        throw std::runtime_error("removeEntry: entry '" + name + "' not found");
}

void DirectoryService::renameEntry(const std::string& old_name, const std::string& new_name,
                                   IFileObject* dir_object)
{
    if (old_name.empty() || new_name.empty())
        throw std::invalid_argument("renameEntry: names cannot be empty");

    if (old_name == new_name)
        return;

    auto dir = dynamic_cast<IDirectory*>(dir_object);
    if (!dir)
        throw std::invalid_argument("renameEntry: not a directory");

    auto dir_structure = dir->getDirectoryStructure();

    if (!dir_structure->contains(old_name))
        throw std::runtime_error("renameEntry: entry '" + old_name + "' not found");

    if (dir_structure->contains(new_name))
        throw std::runtime_error("renameEntry: entry '" + new_name + "' already exists");

    dir_structure->renameEntry(old_name, new_name);
}

IFileObject* DirectoryService::findEntry(const std::string& name,
                                         IFileObject* dir_object)
{
    if (name.empty()) throw std::invalid_argument("findEntry: name is empty");

    auto dir = dynamic_cast<IDirectory*>(dir_object);
    if (!dir)
        throw std::invalid_argument("findEntry: not a directory");

    IFileObject* entry = dir->getDirectoryStructure()->get(name);
    if (!entry)
        throw std::runtime_error("findEntry: entry '" + name + "' not found");

    return entry;
}

size_t DirectoryService::calculateDirectorySize(IDirectory* dir) {
    if (!dir) {
        throw std::invalid_argument("calculateDirectorySize: dir is null");
    }

    size_t total_size = 0;
    for (auto* obj : dir->getDirectoryStructure()->getObjs()) {
        if (auto* subdir = dynamic_cast<IDirectory*>(obj)) {
            total_size += calculateDirectorySize(subdir);
        } else {
            total_size += obj->getInMemoryFile()->getSize();
        }
    }
    return total_size;
}

void DirectoryService::updateDirectorySizeRecursive(IDirectory* dir, IKernelService* kernel) {
    if (!dir) return;

    size_t new_size = calculateDirectorySize(dir);
    dir->getInMemoryFile()->setSize(new_size);

    if (dir->getPath() == "/") {
        return;
    }

    IFileObject* dir_obj = dynamic_cast<IFileObject*>(dir);
    if (dir_obj && kernel) {
        IDirectory* parent = kernel->resolveParentDir(*dir_obj);
        if (parent && parent != dir) {
            updateDirectorySizeRecursive(parent, kernel);
        }
    }
}

} // namespace services