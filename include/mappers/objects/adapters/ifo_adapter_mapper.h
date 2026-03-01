#pragma once

#include <memory>
#include <typeindex>

namespace domain {
class IFileObject;
}

class FileObjectSnapshot;

/**
 * @class IFileObjectMapperAdapter
 *
 * @brief интерфейс адаптера для преобразования конкретных типов файловых объектов
 *
 * адаптер обеспечивает преобразование между конкретным типом доменного объекта
 * файловой системы и соответствующим типом снапшота. каждый адаптер знает только
 * о своем конкретном типе, что позволяет избежать проверок типов в основном маппере
 */
class IFileObjectMapperAdapter {
public:
    /**
     * @brief деструктор интерфейса адаптера маппера файловых объектов
     */
    virtual ~IFileObjectMapperAdapter() = default;

    /**
     * @brief получить тип доменного объекта, который обрабатывает адаптер
     *
     * @return std::type_index тип доменного объекта
     */
    virtual std::type_index getEntityType() const = 0;

    /**
     * @brief получить тип снапшота, который создает адаптер
     *
     * @return std::type_index тип снапшота
     */
    virtual std::type_index getSnapshotType() const = 0;

    /**
     * @brief преобразовать доменный объект файловой системы в снапшот
     *
     * @param entity ссылка на доменный объект для преобразования
     * @return std::unique_ptr<FileObjectSnapshot> указатель на созданный снапшот
     */
    virtual std::unique_ptr<FileObjectSnapshot>
    toSnapshot(const domain::IFileObject& entity) const = 0;

    /**
     * @brief восстановить доменный объект файловой системы из снапшота
     *
     * @param snapshot ссылка на снапшот для восстановления
     * @return std::unique_ptr<domain::IFileObject> указатель на восстановленный доменный объект
     */
    virtual std::unique_ptr<domain::IFileObject>
    fromSnapshot(const FileObjectSnapshot& snapshot) const = 0;
};