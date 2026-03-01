#pragma once
#include <memory>

#include "../factories/objects/file_object_fac.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/acl_fac.h"

namespace domain {

/**
 * @class ObjectFactories
 * 
 * @brief контейнер для фабрик объектов домена
 * 
 * хранит фабрики для создания объектов файловой системы, пользователей, групп и списков контроля доступа
 */
class ObjectFactories {
public:
    std::unique_ptr<IFileObjectFactory> object_factory;
    std::unique_ptr<IUserFactory> user_factory;
    std::unique_ptr<IGroupFactory> group_factory;
    std::unique_ptr<IACLFactory> acl_factory;

    /**
     * @brief конструктор контейнера фабрик объектов
     * 
     * @param object_factory фабрика объектов файловой системы
     * @param user_factory фабрика пользователей
     * @param group_factory фабрика групп
     * @param acl_factory фабрика списков контроля доступа
     */
    ObjectFactories(
        std::unique_ptr<IFileObjectFactory> object_factory,
        std::unique_ptr<IUserFactory> user_factory,
        std::unique_ptr<IGroupFactory> group_factory,
        std::unique_ptr<IACLFactory> acl_factory
    ) noexcept :
        object_factory(std::move(object_factory)),
        user_factory(std::move(user_factory)),
    group_factory(std::move(group_factory)),
    acl_factory(std::move(acl_factory)) {}
};
}
