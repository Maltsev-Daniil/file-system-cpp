#pragma once

#include <memory>
#include "metadata_mapper.h"
#include "snapshots/objects/regular_file_snapshot.h"

namespace domain {
class IRegularFile;
}

/**
 * @class IRegularFileMapper
 *
 * @brief интерфейс для преобразования доменных объектов обычных файлов в снапшоты
 *
 * обеспечивает двустороннее преобразование между объектами обычных файлов и их
 * сериализуемыми представлениями (снапшотами)
 */
class IRegularFileMapper {
public:
    /**
     * @brief деструктор интерфейса маппера обычных файлов
     */
    virtual ~IRegularFileMapper() = default;

    /**
     * @brief преобразовать доменный объект обычного файла в снапшот
     *
     * @param file ссылка на доменный объект обычного файла для преобразования
     * @return std::unique_ptr<RegularFileSnapshot> указатель на созданный снапшот
     */
    virtual std::unique_ptr<RegularFileSnapshot>
    toSnapshot(const domain::IRegularFile& file) const = 0;

    /**
     * @brief восстановить доменный объект обычного файла из снапшота
     *
     * @param snapshot ссылка на снапшот обычного файла для восстановления
     * @return std::unique_ptr<domain::IRegularFile> указатель на восстановленный доменный объект
     */
    virtual std::unique_ptr<domain::IRegularFile>
    fromSnapshot(const RegularFileSnapshot& snapshot) const = 0;
};

/**
 * @class RegularFileMapper
 *
 * @brief реализация маппера для обычных файлов
 *
 * выполняет преобразование между объектами обычных файлов и их снапшотами,
 * используя зависимость для работы с метаданными файлов
 */
class RegularFileMapper : public IRegularFileMapper {
public:
    /**
     * @brief конструктор маппера обычных файлов
     *
     * @param metadata_mapper маппер для метаданных файлов
     */
    explicit RegularFileMapper(IFileMetadataMapper& metadata_mapper) :
        metadata_mapper_(metadata_mapper) {}

    /**
     * @brief преобразовать доменный объект обычного файла в снапшот
     *
     * сериализует обычный файл, включая его содержимое и метаданные
     *
     * @param file ссылка на доменный объект обычного файла для преобразования
     * @return std::unique_ptr<RegularFileSnapshot> указатель на созданный снапшот
     */
    std::unique_ptr<RegularFileSnapshot>
    toSnapshot(const domain::IRegularFile& file) const override;

    /**
     * @brief восстановить доменный объект обычного файла из снапшота
     *
     * восстанавливает обычный файл, включая его содержимое и метаданные
     *
     * @param snapshot ссылка на снапшот обычного файла для восстановления
     * @return std::unique_ptr<domain::IRegularFile> указатель на восстановленный доменный объект
     */
    std::unique_ptr<domain::IRegularFile>
    fromSnapshot(const RegularFileSnapshot& snapshot) const override;

private:
    IFileMetadataMapper& metadata_mapper_;
};