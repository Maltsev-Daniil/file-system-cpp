//
// Created by nitsir on 1/4/26.
//


#include "containers/persistence_container.h"

#include "mappers/acl/acl_mapper.h"
#include "mappers/entity/group_mapper.h"
#include "mappers/entity/user_mapper.h"
#include "mappers/file_system/file_system_mapper.h"
#include "mappers/objects/file_object_mapper.h"
#include "mappers/objects/metadata_mapper.h"
#include "mappers/objects/regfile_mapper.h"
#include "mappers/objects/adapters/directory_adapter_mapper.h"
#include "mappers/objects/adapters/ifo_adapter_mapper.h"
#include "mappers/objects/adapters/regfile_adapter_mapper.h"
#include "serializer/yaml_serializer.h"

PersistenceContainer::PersistenceContainer(
    domain::IACLFactory& acl_factory,
    domain::IInMemoryFactory& in_memory_factory
) {
    acl_mapper_ = std::make_unique<AclMapper>(acl_factory);
    metadata_mapper_ =
        std::make_unique<FileMetadataMapper>(*acl_mapper_, in_memory_factory);

    regular_file_mapper_ =
        std::make_unique<RegularFileMapper>(*metadata_mapper_);

    object_mapper_ =
        std::make_unique<PolymorphicFileObjectMapper>();

    directory_mapper_ =
        std::make_unique<DirectoryMapper>(*object_mapper_, *metadata_mapper_);

    auto dir_adapter = std::make_unique<DirectoryMapperAdapter>(*directory_mapper_);
    object_mapper_->registerAdapter(std::move(dir_adapter));
    auto regfile_adapter = std::make_unique<RegularFileMapperAdapter>(*regular_file_mapper_);
    object_mapper_->registerAdapter(std::move(regfile_adapter));

    user_mapper_ = std::make_unique<UserMapper>();
    group_mapper_ = std::make_unique<GroupMapper>();

    fs_mapper_ = std::make_unique<FileSystemMapper>(
        *user_mapper_, *group_mapper_, *directory_mapper_
    );

    serializer_ = std::make_unique<YamlFileSystemSerializer>();
}
