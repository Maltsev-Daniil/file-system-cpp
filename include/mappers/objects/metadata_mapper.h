#pragma once

#include <memory>
#include "snapshots/objects/file_metadata_snapshot.h"

namespace domain {
class IInMemoryFactory;
class IInMemoryFile;
class FileMetadata;
}

class IAclMapper;

/**
 * @class IFileMetadataMapper
 *
 * @brief интерфейс для преобразования метаданных файлов в снапшоты
 *
 * обеспечивает двустороннее преобразование между объектами метаданных файлов и их
 * сериализуемыми представлениями (снапшотами)
 */
class IFileMetadataMapper {
public:
    /**
     * @brief деструктор интерфейса маппера метаданных файлов
     */
    virtual ~IFileMetadataMapper() = default;

    /**
     * @brief преобразовать доменный объект метаданных файла в снапшот
     *
     * @param metadata ссылка на доменный объект метаданных файла для преобразования
     * @return FileMetadataSnapshot созданный снапшот метаданных
     */
    virtual FileMetadataSnapshot
    toSnapshot(const domain::IInMemoryFile& metadata) const = 0;

    /**
     * @brief восстановить доменный объект метаданных файла из снапшота
     *
     * @param snapshot ссылка на снапшот метаданных для восстановления
     * @return std::unique_ptr<domain::IInMemoryFile> указатель на восстановленный доменный объект
     */
    virtual std::unique_ptr<domain::IInMemoryFile>
    fromSnapshot(const FileMetadataSnapshot& snapshot) const = 0;
};

/**
 * @class FileMetadataMapper
 *
 * @brief реализация маппера для метаданных файлов
 *
 * выполняет преобразование между объектами метаданных файлов и их снапшотами,
 * используя зависимости для работы с ACL и фабрикой in-memory объектов
 */
class FileMetadataMapper final : public IFileMetadataMapper {
public:
    /**
     * @brief конструктор маппера метаданных файлов
     *
     * @param acl_mapper маппер для преобразования ACL
     * @param in_memory_factory фабрика для создания in-memory объектов
     */
    FileMetadataMapper(
        IAclMapper& acl_mapper,
        domain::IInMemoryFactory& in_memory_factory
    ) : acl_mapper_(acl_mapper),
        in_memory_factory_(in_memory_factory) {}

    /**
     * @brief преобразовать доменный объект метаданных файла в снапшот
     *
     * сериализует метаданные файла, включая права доступа (ACL) и атрибуты
     *
     * @param metadata ссылка на доменный объект метаданных файла для преобразования
     * @return FileMetadataSnapshot созданный снапшот метаданных
     */
    FileMetadataSnapshot
    toSnapshot(const domain::IInMemoryFile& metadata) const override;

    /**
     * @brief восстановить доменный объект метаданных файла из снапшота
     *
     * восстанавливает метаданные файла, включая права доступа (ACL) и атрибуты
     *
     * @param snapshot ссылка на снапшот метаданных для восстановления
     * @return std::unique_ptr<domain::IInMemoryFile> указатель на восстановленный доменный объект
     */
    std::unique_ptr<domain::IInMemoryFile>
    fromSnapshot(const FileMetadataSnapshot& snapshot) const override;

private:
    IAclMapper& acl_mapper_;
    domain::IInMemoryFactory& in_memory_factory_;
};