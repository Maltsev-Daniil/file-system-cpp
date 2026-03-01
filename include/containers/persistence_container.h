#pragma once

#include <memory>

#include "serializer/iserializer.h"
#include "mappers/objects/adapters/ifo_adapter_mapper.h"

class PolymorphicFileObjectMapper;
class IGroupMapper;
class IUserMapper;
class IDirectoryMapper;
class IFileObjectMapper;
class IRegularFileMapper;
class IFileMetadataMapper;
class IAclMapper;

namespace domain {
class IInMemoryFactory;
class IACLFactory;
}

class FileSystemSnapshot;
class IFileSystemMapper;

/**
 * @class IPersistenceContainer
 * 
 * @brief интерфейс для контейнера персистентности
 * 
 * предоставляет доступ к мапперу файловой системы и сериализатору
 */
class IPersistenceContainer {
public:
    /**
     * @brief деструктор интерфейса контейнера персистентности
     */
    virtual ~IPersistenceContainer() = default;

    /**
     * @brief получить маппер файловой системы
     * 
     * @return IFileSystemMapper* указатель на маппер файловой системы
     */
    virtual IFileSystemMapper* getFsMapper() = 0;

    /**
     * @brief получить сериализатор файловой системы
     * 
     * @return ISerializer<FileSystemSnapshot>* указатель на сериализатор
     */
    virtual ISerializer<FileSystemSnapshot>* getSerializer() = 0;
};

/**
 * @class PersistenceContainer
 * 
 * @brief реализация контейнера персистентности
 * 
 * создает и хранит мапперы и сериализатор для работы с персистентностью файловой системы
 */
class PersistenceContainer final : public IPersistenceContainer {
public:
    /**
     * @brief конструктор контейнера персистентности
     * 
     * @param acl_factory фабрика списков контроля доступа
     * @param in_memory_factory фабрика in-memory файлов
     */
    PersistenceContainer(
        domain::IACLFactory& acl_factory,
        domain::IInMemoryFactory& in_memory_factory
    );

    /**
     * @brief получить маппер файловой системы
     * 
     * @return IFileSystemMapper* указатель на маппер файловой системы
     */
    IFileSystemMapper* getFsMapper() override {
        return fs_mapper_.get();
    }

    /**
     * @brief получить сериализатор файловой системы
     * 
     * @return ISerializer<FileSystemSnapshot>* указатель на сериализатор
     */
    ISerializer<FileSystemSnapshot>* getSerializer() override {
        return serializer_.get();
    }

private:
    std::unique_ptr<IAclMapper> acl_mapper_;
    std::unique_ptr<IFileMetadataMapper> metadata_mapper_;
    std::unique_ptr<IRegularFileMapper> regular_file_mapper_;
    std::unique_ptr<PolymorphicFileObjectMapper> object_mapper_;
    std::unique_ptr<IDirectoryMapper> directory_mapper_;
    std::unique_ptr<IUserMapper> user_mapper_;
    std::unique_ptr<IGroupMapper> group_mapper_;
    std::unique_ptr<IFileSystemMapper> fs_mapper_;

    std::unique_ptr<ISerializer<FileSystemSnapshot>> serializer_;
};
