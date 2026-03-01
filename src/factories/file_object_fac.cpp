
#include "factories/objects/file_object_fac.h"

#include <memory>

#include "domain/objects/directory.h"
#include "domain/objects/directory_structure.h"
#include "domain/objects/file_object.h"
#include "domain/objects/inmem_file.h"
#include "domain/objects/reg_file.h"

namespace domain {
std::unique_ptr<IFileObject> FileFactory::createObject(
const std::string& name,
const std::string& path,
std::unique_ptr<IInMemoryFile> in_memory)
{
    return std::make_unique<RegularFile>(
        std::move(in_memory),
        name,
        path
    );
}

std::unique_ptr<IFileObject> DirectoryFactory::createObject(
    const std::string& name,
    const std::string& path,
    std::unique_ptr<IInMemoryFile> in_memory)
{
    return std::make_unique<Directory>(
        std::move(in_memory),
        std::make_unique<DirectoryStructure>(),
        name,
        path
    );
}
}
