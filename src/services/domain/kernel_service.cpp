#include "services/domain/kernel_service.h"
#include "domain/tables/itable.h"
#include "containers/kernel_container.h"
#include "../../../include/domain/objects/directory_structure.h"
#include "domain/objects/directory.h"

#include <sstream>
#include <cassert>
#include <algorithm>

#include "../../../debug.h"
#include "domain/objects/inmem_file.h"
#include "domain/objects/reg_file.h"
#include "../../../include/factories/objects/inmem_fac.h"

using namespace domain;

namespace services {

std::vector<std::string> KernelService::splitPath(const std::string& path) const {
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string token;

    while (std::getline(ss, token, '/')) {
        if (!token.empty())
            parts.push_back(token);
    }
    return parts;
}

std::vector<std::string> KernelService::normalizePathParts(
    const std::vector<std::string>& parts
) const {
    std::vector<std::string> result;
    result.reserve(parts.size());

    for (const auto& p : parts) {
        if (p == "." || p.empty()) {
            continue;
        }
        if (p == "..") {
            if (!result.empty()) {
                result.pop_back();
            }
            // попытка выйти выше корня игнорируется
            continue;
        }
        result.push_back(p);
    }
    return result;
}

IDirectory* KernelService::resolveDirectory(
    const std::vector<std::string>& normalized_parts
) const {
    auto* dir_root = container->getRootDirectory().get();
    if (!dir_root) {
        throw std::runtime_error("resolveDirectory: root directory is null");
    }
    auto* current = dynamic_cast<IDirectory*>(dir_root);
    if (!current) {
        throw std::logic_error(
            "resolveDirectory: root file object is not a directory"
        );
    }
    if (normalized_parts.empty()) {
        return current;
    }

    for (const auto& seg : normalized_parts) {
        auto* ds = current->getDirectoryStructure();
        if (!ds) {
            throw std::runtime_error("resolveDirectory: directory has no structure");
        }

        IFileObject* obj = ds->get(seg);
        if (!obj) {
            throw std::runtime_error("resolveDirectory: segment '" + seg + "' not found");
        }

        auto* next = dynamic_cast<IDirectory*>(obj);
        if (!next) {
            throw std::runtime_error(
                "resolveDirectory: segment '" + seg + "' is not a directory"
            );
        }
        current = next;
    }
    return current;
}

namespace {

bool matchNameImpl(const std::string& pat, const std::string& str,
                   size_t pi, size_t si) {
    const size_t pn = pat.size();
    const size_t sn = str.size();

    if (pi == pn) {
        return si == sn;
    }

    char pc = pat[pi];

    if (pc == '*') {
        for (size_t k = si; k <= sn; ++k) {
            if (matchNameImpl(pat, str, pi + 1, k)) {
                return true;
            }
        }
        return false;
    }

    if (si == sn) {
        return false;
    }

    if (pc == '?') {
        return matchNameImpl(pat, str, pi + 1, si + 1);
    }

    if (pc != str[si]) {
        return false;
    }
    return matchNameImpl(pat, str, pi + 1, si + 1);
}

} // namespace

bool KernelService::matchName(
    const std::string& pattern,
    const std::string& name
) const {
    return matchNameImpl(pattern, name, 0, 0);
}

std::vector<std::string> KernelService::expandLastPart(
    IDirectory* base_dir,
    const std::string& name_pattern
) const {
    if (!base_dir) {
        throw std::invalid_argument("expandLastPart: base_dir is null");
    }

    auto* ds = base_dir->getDirectoryStructure();
    if (!ds) {
        throw std::runtime_error("expandLastPart: directory has no structure");
    }

    std::vector<std::string> result;
    for (const auto& name : ds->listNames()) {
        if (matchName(name_pattern, name)) {
            result.push_back(name);
        }
    }

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

std::vector<std::string> KernelService::expandAbsolutePattern(
    const std::string& absolute_pattern
) const {
    if (absolute_pattern.empty() || absolute_pattern[0] != '/') {
        throw std::invalid_argument("expandAbsolutePattern: path must be absolute");
    }

    auto parts = splitPath(absolute_pattern);
    auto norm = normalizePathParts(parts);

    if (norm.empty()) {
        return {"/"};
    }

    std::vector<std::string> dir_parts(norm.begin(), norm.end() - 1);
    const std::string& name_pattern = norm.back();

    IFileObject* root_dir = nullptr;
    IDirectory* base_dir = nullptr;
    if (dir_parts.empty()) {
        root_dir = container->getRootDirectory().get();
        if (!root_dir) {
            throw std::runtime_error("expandAbsolutePattern: root directory is null");
        }
        base_dir = dynamic_cast<IDirectory*>(root_dir);
        if (!base_dir) {
            throw std::runtime_error("expandAbsolutePattern: root is not a directory");
        }
    } else {
        base_dir = resolveDirectory(dir_parts);
        if (!base_dir) {
            throw std::runtime_error("expandAbsolutePattern: cannot resolve base directory");
        }
    }

    auto names = expandLastPart(base_dir, name_pattern);
    if (names.empty()) {
        return {};
    }

    std::string base_path = "/";
    if (!dir_parts.empty()) {
        for (size_t i = 0; i < dir_parts.size(); ++i) {
            base_path += dir_parts[i];
            if (i + 1 < dir_parts.size()) {
                base_path += "/";
            }
        }
        if (base_path.back() != '/') {
            base_path += "/";
        }
    }

    std::vector<std::string> full_paths;
    full_paths.reserve(names.size());
    for (const auto& name : names) {
        full_paths.push_back(base_path + name);
    }

    std::sort(full_paths.begin(), full_paths.end());
    return full_paths;
}

void KernelService::setIdToCopiedObj(IFileObject *obj, userId_t uid, groupId_t gid)
{
    inmem_fac->generateId(obj->getInMemoryFile());
    obj->getInMemoryFile()->setUid(uid);
    obj->getInMemoryFile()->setGid(gid);

    if (auto* dir = dynamic_cast<IDirectory*>(obj)) {
        auto* dir_struct = dir->getDirectoryStructure();
        for (const auto& name : dir_struct->listNames()) {
            auto* obj_inside = dir_struct->get(name);
            setIdToCopiedObj(obj_inside, uid, gid);
        }
    }
}

domain::IDirectory *KernelService::getRootDir()
{
    return dynamic_cast<domain::IDirectory *>(container->getRootDirectory().get());
}

void KernelService::updateModTime(IFileObject* obj)
{
    if (!obj) {
        throw std::runtime_error("updateModTime: obj is null");
    }

    obj->getInMemoryFile()->updateModificationTime();

    if (obj->getPath() == "/") {
        return;
    }

    IDirectory* parent = resolveParentDir(*obj);
    if (parent && parent != obj) {
        updateModTime(parent);
    }
}

IDirectory* KernelService::resolveParentDir(domain::IFileObject &obj) {
    auto parts = splitPath(obj.getPath());
    if (parts.empty()) {
        throw std::runtime_error("resolveParentDir: object has invalid path");
    }
    std::vector<std::string> dir_parts(parts.begin(), parts.end() - 1);

    if (dir_parts.empty()) {
        return this->getRootDir();
    }

    auto base_dir = resolveDirectory(dir_parts);
    if (!base_dir) {
        throw std::runtime_error(
            "resolveParentDir: cannot resolve parent directory for path: " + obj.getPath()
        );
    }
    return base_dir;
}

IFileObject* KernelService::resolvePath(const std::string& path) {
    assert(!path.empty() && "resolvePath: path must not be empty");
    assert(path[0] == '/' && "resolvePath: path must be absolute");
    assert(container && "KernelService: user_table must not be null");

    IFileObject* root = container->getRootDirectory().get();
    assert(root && "KernelService: root directory is null");

    if (path == "/") {
        return root;
    }

    auto parts = splitPath(path);
    IDirectory* current = dynamic_cast<IDirectory*>(root);
    if (!current) {
        throw std::logic_error("resolvePath: root file object is not a directory");
    }
    std::string prefix = "/";

    for (size_t i = 0; i < parts.size(); ++i) {
        const std::string& seg = parts[i];
        DEBUG_LOG("segment is " + seg);

        auto* ds = current->getDirectoryStructure();
        if (!ds) {
            throw std::runtime_error(
                "resolvePath: directory has no structure at '" + prefix + "'"
            );
        }

        IFileObject* obj = ds->get(seg);
        if (!obj) {
            throw std::runtime_error(
                "resolvePath: segment '" + seg + "' not found in '" + prefix + "'"
            );
        }

        if (i + 1 == parts.size()) {
            return obj;
        }

        auto* next = dynamic_cast<IDirectory*>(obj);
        if (!next) {
            throw std::runtime_error(
                "resolvePath: '" + seg + "' is not a directory in path '" + path + "'"
            );
        }

        prefix += seg + "/";
        current = next;
    }

    assert(false && "Unreachable");
    return nullptr;
}

IUser* KernelService::resolveUser(const std::string& username) {
    assert(container && "KernelService: user_table is null");
    assert(!username.empty() && "resolveUser: username must not be empty");

    auto* ut = container->getUserTable().get();
    assert(ut && "KernelService: user table is null");

    IUser* u = ut->getByName(username);
    if (!u) {
        throw std::runtime_error("resolveUser: user '" + username + "' not found");
    }

    return u;
}

IGroup* KernelService::resolveGroup(const std::string& groupname) {
    assert(container && "KernelService: user_table is null");
    assert(!groupname.empty() && "resolveGroup: groupname must not be empty");

    auto* gt = container->getGroupTable().get();
    assert(gt && "KernelService: group table is null");

    IGroup* g = gt->getByName(groupname);
    if (!g) {
        throw std::runtime_error("resolveGroup: group '" + groupname + "' not found");
    }

    return g;
}

domain::IUser *KernelService::resolveUser(domain::userId_t uid)
{
    auto* ut = container->getUserTable().get();
    assert(ut && "KernelService: user table is null");

    IUser* u = ut->getById(uid);
    if (!u) {
        throw std::runtime_error("resolveUser: user with id '" + std::to_string(uid) + "' not found");
    }

    return u;
}

domain::IGroup *KernelService::resolveGroup(domain::groupId_t gid)
{
    auto* gt = container->getGroupTable().get();
    assert(gt && "KernelService: group table is null");

    IGroup* g = gt->getById(gid);
    if (!g) {
        throw std::runtime_error("resolveGroup: group with id '" + std::to_string(gid) + "' not found");
    }

    return g;
}

// domain::IUser *KernelService::resolveUser(domain::userId_t uid)
// {
//     auto* ut = container->getUserTable();
    
// }

IInMemoryFile *KernelService::getInMemory(IFileObject *obj)
{
    return obj->getInMemoryFile();
}

void KernelService::move(IFileObject *obj, IFileObject *old_directory, IFileObject *new_directory)
{
    auto* new_dir = dynamic_cast<IDirectory*>(new_directory);
    if (!new_dir) throw std::runtime_error("move: no destination directory");
    if (new_dir->getDirectoryStructure()->contains(obj->getName()))
        throw std::runtime_error("move: dist dir contains obj with same name");
    
    auto* old_dir = dynamic_cast<IDirectory*>(old_directory);
    if (!old_dir) throw std::runtime_error("move: no parent directory");

    std::unique_ptr<IFileObject> obj_own = old_dir->getDirectoryStructure()->extract(obj->getName());
    if (!obj_own)
        throw std::runtime_error("move: parent dir missing object to move");

    // обновляем путь
    obj->updatePathRecursive(new_directory->getPath());

    if (!new_dir->getDirectoryStructure()->addEntry(std::move(obj_own)))
        throw std::runtime_error("move: addEntry failed");

}

std::unique_ptr<IFileObject> KernelService::copy(IFileObject *obj, 
    userId_t uid, groupId_t gid)
{
    std::unique_ptr<IFileObject> obj_clone = obj->clone();
    setIdToCopiedObj(obj_clone.get(), uid, gid);
    return obj_clone;
}

void KernelService::remove(IFileObject *object, IFileObject *parent_dir_obj)
{
    if (!object) throw std::invalid_argument("remove: ob is null");
    if (!parent_dir_obj) throw std::invalid_argument("remove: parent_dir is null");

    auto parent_dir = dynamic_cast<IDirectory*>(parent_dir_obj);
    if (!parent_dir)
        throw std::invalid_argument("remove: not a parent directory");
    auto parent_dir_struct = parent_dir->getDirectoryStructure();

    if (auto* dir = dynamic_cast<IDirectory*>(object)) {
        auto dir_struct = dir->getDirectoryStructure();

        if (dir_struct->size() > 0)
            throw std::runtime_error("remove: directory is not empty");

        if (!parent_dir_struct->removeEntry(dir->getName()))
            throw std::runtime_error("remove: dir " + dir->getName() + " not found");
    }

    else if (auto* reg_file = dynamic_cast<IRegularFile*>(object)) {
        if (!parent_dir_struct->removeEntry(reg_file->getName()))
            throw std::runtime_error("remove: file " + reg_file->getName() + " not found");
    } else {
        throw std::invalid_argument("Unsupported file object type");
    }
}

} // namespace services
