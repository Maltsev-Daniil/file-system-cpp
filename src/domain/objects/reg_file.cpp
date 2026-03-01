//
// Created by nitsir on 12/24/25.
//

#include "../../../include/domain/objects/reg_file.h"
#include "../../../include/domain/objects/inmem_file.h"

#include <memory>

namespace domain {
RegularFile::RegularFile(std::unique_ptr<IInMemoryFile> in_memory_file,
                         const std::string& name,
                         const std::string& path,
                         LockType lock_type,
                         const std::string& initial_content)
    : in_memory(std::move(in_memory_file))
    , name(name)
    , path(path)
    , lock(lock_type)
    , content(initial_content)
{
}

void RegularFile::updatePathRecursive(const std::string &parent_path) {
    setPath(parent_path + "/" + name);
}

std::unique_ptr<IFileObject> RegularFile::clone() const
{
    auto copy = std::make_unique<RegularFile>(
        this->in_memory->clone(),
        this->name,
        this->path,
        this->lock,
        this->content
    );

    return copy;
}
}
