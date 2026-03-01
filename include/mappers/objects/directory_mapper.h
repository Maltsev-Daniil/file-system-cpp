#pragma once

#include <memory>
#include "snapshots/objects/directory_snapshot.h"

class IFileMetadataMapper;
class DirectorySnapshot;
class IFileObjectMapper;

namespace domain {
class IDirectory;
}

/**
 * @class IDirectoryMapper
 *
 * @brief интерфейс для преобразования доменных объектов директорий в снапшоты
 *
 * обеспечивает двустороннее преобразование между объектами директорий и их
 * сериализуемыми представлениями (снапшотами)
 */
class IDirectoryMapper {
public:
    /**
     * @brief деструктор интерфейса маппера директорий
     */
    virtual ~IDirectoryMapper() = default;

    /**
     * @brief преобразовать доменный объект директории в снапшот
     *
     * @param directory ссылка на доменный объект директории для преобразования
     * @return std::unique_ptr<DirectorySnapshot> указатель на созданный снапшот
     */
    virtual std::unique_ptr<DirectorySnapshot>
    toSnapshot(const domain::IDirectory& directory) const = 0;

    /**
     * @brief восстановить доменный объект директории из снапшота
     *
     * @param snapshot ссылка на снапшот директории для восстановления
     * @return std::unique_ptr<domain::IDirectory> указатель на восстановленный доменный объект
     */
    virtual std::unique_ptr<domain::IDirectory>
    fromSnapshot(const DirectorySnapshot& snapshot) const = 0;
};

/**
 * @class DirectoryMapper
 *
 * @brief реализация маппера для директорий
 *
 * выполняет преобразование между объектами директорий и их снапшотами,
 * используя зависимости для работы с метаданными и другими файловыми объектами
 */
class DirectoryMapper : public IDirectoryMapper {
public:
    /**
     * @brief конструктор маппера директорий
     *
     * @param file_object_mapper маппер для дочерних файловых объектов
     * @param metadata_mapper маппер для метаданных директории
     */
    DirectoryMapper(
        IFileObjectMapper& file_object_mapper,
        IFileMetadataMapper& metadata_mapper
    ) : file_object_mapper_(file_object_mapper),
        metadata_mapper_(metadata_mapper) {}

    /**
     * @brief преобразовать доменный объект директории в снапшот
     *
     * сериализует содержимое директории, включая дочерние объекты и метаданные
     *
     * @param directory ссылка на доменный объект директории для преобразования
     * @return std::unique_ptr<DirectorySnapshot> указатель на созданный снапшот
     */
    std::unique_ptr<DirectorySnapshot>
    toSnapshot(const domain::IDirectory& directory) const override;

    /**
     * @brief восстановить доменный объект директории из снапшота
     *
     * восстанавливает директорию, включая все дочерние объекты и метаданные
     *
     * @param snapshot ссылка на снапшот директории для восстановления
     * @return std::unique_ptr<domain::IDirectory> указатель на восстановленный доменный объект
     */
    std::unique_ptr<domain::IDirectory>
    fromSnapshot(const DirectorySnapshot& snapshot) const override;

private:
    IFileObjectMapper& file_object_mapper_;
    IFileMetadataMapper& metadata_mapper_;
};