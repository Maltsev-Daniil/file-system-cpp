#pragma once

#include <memory>

#include "domain/types.h"

namespace domain {
class IGroup;
class IUser;

/**
 * @class IEntityTable
 * 
 * @brief интерфейс для таблицы сущностей
 * 
 * определяет операции для работы с таблицей сущностей по имени и идентификатору
 * 
 * @tparam TObject тип объекта в таблице
 * @tparam TId тип идентификатора объекта
 */
template <
    typename TObject,
    typename TId>
class IEntityTable {
public:
    /**
     * @brief деструктор интерфейса таблицы сущностей
     */
    virtual ~IEntityTable() = default;

    /**
     * @brief добавить объект в таблицу
     * 
     * @param object объект для добавления
     * @return true если объект добавлен, false если уже существует
     */
    virtual bool add(std::unique_ptr<TObject> object) = 0;

    /**
     * @brief удалить объект из таблицы по имени
     * 
     * @param name имя объекта
     * @return true если объект удален, false если не найден
     */
    virtual bool remove(const std::string& name) = 0;

    /**
     * @brief получить объект по имени
     * 
     * @param name имя объекта
     * @return TObject* указатель на объект или nullptr если не найден
     */
    virtual TObject* getByName(const std::string& name) const = 0;

    /**
     * @brief получить объект по идентификатору
     * 
     * @param id идентификатор объекта
     * @return TObject* указатель на объект или nullptr если не найден
     */
    virtual TObject* getById(TId id) const = 0;

    /**
     * @brief получить все объекты из таблицы
     * 
     * @return std::vector<const TObject*> вектор указателей на объекты
     */
    virtual std::vector<const TObject*> getItems() const = 0;

    /**
     * @brief проверить наличие объекта по имени
     * 
     * @param name имя объекта
     * @return true если объект существует, false иначе
     */
    virtual bool contains(const std::string& name) const = 0;

    /**
     * @brief проверить наличие объекта по идентификатору
     * 
     * @param id идентификатор объекта
     * @return true если объект существует, false иначе
     */
    virtual bool contains(TId id) const = 0;

    /**
     * @brief получить количество объектов в таблице
     * 
     * @return size_t количество объектов
     */
    virtual size_t size() const = 0;
};


using IUserTable = IEntityTable<IUser, domain::userId_t>;
using IGroupTable = IEntityTable<IGroup, domain::groupId_t>;
}
