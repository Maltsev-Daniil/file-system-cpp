#pragma once

#include <functional>

#include "snapshots/objects/file_object_snapshot.h"
#include "directory_mapper.h"

#include <memory>
#include <typeindex>

#include "regfile_mapper.h"


class IFileObjectMapperAdapter;
class FileObjectSnapshot;

namespace domain {
class IFileObject;
}

/**
 * @class IFileObjectMapper
 *
 * @brief интерфейс для преобразования доменных объектов файловой системы в снапшоты
 *
 * обеспечивает двустороннее преобразование между объектами файловой системы и их
 * сериализуемыми представлениями (снапшотами)
 */
class IFileObjectMapper {
public:
    /**
     * @brief деструктор интерфейса маппера файловых объектов
     */
    virtual ~IFileObjectMapper() = default;

    /**
     * @brief преобразовать доменный объект файловой системы в снапшот
     *
     * @param object ссылка на доменный объект для преобразования
     * @return std::unique_ptr<FileObjectSnapshot> указатель на созданный снапшот
     */
    virtual std::unique_ptr<FileObjectSnapshot>
    toSnapshot(const domain::IFileObject& object) const = 0;

    /**
     * @brief восстановить доменный объект файловой системы из снапшота
     *
     * @param snapshot ссылка на снапшот для восстановления
     * @return std::unique_ptr<domain::IFileObject> указатель на восстановленный доменный объект
     */
    virtual std::unique_ptr<domain::IFileObject>
    fromSnapshot(const FileObjectSnapshot& snapshot) const = 0;
};

/**
 * @class PolymorphicFileObjectMapper
 *
 * @brief полиморфная реализация маппера файловых объектов
 *
 * поддерживает множественные типы файловых объектов через систему адаптеров.
 * автоматически выбирает соответствующий адаптер на основе типа объекта или снапшота
 */
class PolymorphicFileObjectMapper final : public IFileObjectMapper {
public:
    /**
     * @brief зарегистрировать адаптер для конкретного типа файлового объекта
     *
     * @param adapter указатель на адаптер для регистрации
     * @note владение адаптером передается мапперу
     */
    void registerAdapter(std::unique_ptr<IFileObjectMapperAdapter> adapter);

    /**
     * @brief преобразовать доменный объект файловой системы в снапшот
     *
     * автоматически выбирает адаптер на основе типа переданного объекта.
     * если адаптер для данного типа не зарегистрирован, выбрасывает исключение
     *
     * @param object ссылка на доменный объект для преобразования
     * @return std::unique_ptr<FileObjectSnapshot> указатель на созданный снапшот
     * @throws std::logic_error если адаптер для данного типа не найден
     */
    std::unique_ptr<FileObjectSnapshot>
    toSnapshot(const domain::IFileObject& object) const override;

    /**
     * @brief восстановить доменный объект файловой системы из снапшота
     *
     * автоматически выбирает адаптер на основе типа переданного снапшота.
     * если адаптер для данного типа не зарегистрирован, выбрасывает исключение
     *
     * @param snapshot ссылка на снапшот для восстановления
     * @return std::unique_ptr<domain::IFileObject> указатель на восстановленный доменный объект
     * @throws std::logic_error если адаптер для данного типа не найден
     */
    std::unique_ptr<domain::IFileObject>
    fromSnapshot(const FileObjectSnapshot& snapshot) const override;

private:
    std::vector<std::unique_ptr<IFileObjectMapperAdapter>> adapters_;
    std::unordered_map<std::type_index, IFileObjectMapperAdapter*> by_entity_;
    std::unordered_map<std::type_index, IFileObjectMapperAdapter*> by_snapshot_;
};