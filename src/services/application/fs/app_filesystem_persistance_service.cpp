

#include "domain/tables/itable.h"
#include "../../../../include/services/application/app_filesystem_persistance_service.h"

#include "domain/objects/file_object.h"
#include "domain/objects/directory.h"

#include "containers/kernel_container.h"
#include "containers/system_container.h"
#include "factories/init/kernel_fac.h"
#include "mappers/file_system/file_system_mapper.h"

#include "serializer/yaml_serializer.h"

namespace services {

void FileSystemPersistenceAppService::save(const std::string& path)
{
    if (path.empty()) {
        throw std::invalid_argument("FileSystemPersistenceAppService::save: path is empty");
    }

    const auto* kernel = system_container_.getKernelContainer();
    if (!kernel) {
        throw std::runtime_error("FileSystemPersistenceAppService::save: kernel container is null");
    }

    auto* root_dir_obj = kernel->getRootDirectory().get();
    if (!root_dir_obj) {
        throw std::runtime_error("FileSystemPersistenceAppService::save: root directory is null");
    }

    auto* root_dir = dynamic_cast<domain::IDirectory*>(root_dir_obj);
    if (!root_dir) {
        throw std::runtime_error("FileSystemPersistenceAppService::save: root is not a directory");
    }

    auto snapshot = snapshot_mapper_.toSnapshot(
        *root_dir,
        kernel->getUserTable()->getItems(),
        kernel->getGroupTable()->getItems()
    );

    serializer_.write(path, snapshot);
}

void FileSystemPersistenceAppService::load(const std::string& path)
{
    if (path.empty()) {
        throw std::invalid_argument("FileSystemPersistenceAppService::load: path is empty");
    }

    auto snapshot = serializer_.read(path);

    auto kernel =
        snapshot_mapper_.kernelFromSnapshot(snapshot, kernel_factory_);
    
    if (!kernel) {
        throw std::runtime_error("FileSystemPersistenceAppService::load: failed to create kernel from snapshot");
    }

    system_container_.replaceKernelContainer(
        std::shared_ptr<domain::IKernelContainer>(std::move(kernel)));
}

} // namespace services