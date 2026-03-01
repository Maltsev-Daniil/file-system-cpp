//
// Created by nitsir on 12/24/25.
//

#include "../../../include/domain/objects/directory.h"
#include "../../../include/domain/objects/inmem_file.h"
#include "../../../include/domain/objects/directory_structure.h"

namespace domain {
Directory::Directory(std::unique_ptr<IInMemoryFile> in_memory_file,
                     std::unique_ptr<IDirectoryStructure> directory_structure,
                     const std::string& name,
                     const std::string& path)
    : in_memory(std::move(in_memory_file))
    , dir_struct(std::move(directory_structure))
    , name(name)
    , path(path)
{
}

Directory::~Directory() = default;

void Directory::updatePathRecursive(const std::string &parent_path) {
    setPath(parent_path + "/" + name);
    for (auto& child : dir_struct->getObjs()) {
        child->updatePathRecursive(this->getPath());
    }
}

std::unique_ptr<IFileObject> Directory::clone() const
{
    auto copy = std::make_unique<Directory>(
        this->in_memory->clone(),
        this->dir_struct->clone(),
        this->name,
        this->path
    );
    return copy;
}
}
