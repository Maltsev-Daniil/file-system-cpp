#pragma once

#include "domain/types.h"
#include <memory>

#include "kernel_fac_validator.h"
#include "domain/tables/itable.h"

namespace domain {

class IDirectory;
class IDirectoryStructure;
class IUserFactory;
class IGroupFactory;
class IACLFactory;
class IFileObjectFactory;
class IInMemoryFactory;
class IKernelContainer;
class IFileObject;

/**
 * @class IKernelFactory
 * 
 * @brief интерфейс для фабрики ядра файловой системы
 * 
 * предоставляет методы для создания таблиц пользователей и групп, корневой директории и контейнера ядра
 */
class IKernelFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики ядра
     */
    virtual ~IKernelFactory() = default;

    /**
     * @brief создать таблицу пользователей с администратором
     * 
     * @param user_factory фабрика пользователей
     * @return std::unique_ptr<IUserTable> созданная таблица пользователей
     */
    virtual std::unique_ptr<IUserTable> createTabUserWithAdmin(std::shared_ptr<IUserFactory> user_factory) = 0;

    /**
     * @brief создать таблицу групп с группой администраторов
     * 
     * @param group_factory фабрика групп
     * @return std::unique_ptr<IGroupTable> созданная таблица групп
     */
    virtual std::unique_ptr<IGroupTable> createTabGroupWithAdmin(std::shared_ptr<IGroupFactory> group_factory) = 0;

    /**
     * @brief создать пустую таблицу пользователей
     * 
     * @return std::unique_ptr<IUserTable> созданная таблица пользователей
     */
    virtual std::unique_ptr<IUserTable>
        createEmptyTabUser() = 0;

    /**
     * @brief создать пустую таблицу групп
     * 
     * @return std::unique_ptr<IGroupTable> созданная таблица групп
     */
    virtual std::unique_ptr<IGroupTable>
        createEmptyTabGroup() = 0;

    /**
     * @brief создать корневую директорию
     * 
     * @param obj_factory фабрика объектов файловой системы
     * @param inmem_factory фабрика in-memory файлов
     * @param acl_factory фабрика списков контроля доступа
     * @param admin_id идентификатор администратора
     * @param group_admins_id идентификатор группы администраторов
     * @return std::unique_ptr<IFileObject> созданная корневая директория
     */
    virtual std::unique_ptr<IFileObject> createDirRoot(
        std::shared_ptr<IFileObjectFactory> obj_factory,
        std::shared_ptr<IInMemoryFactory> inmem_factory,
        std::shared_ptr<IACLFactory> acl_factory,
        userId_t admin_id,
        groupId_t group_admins_id) = 0;

    /**
     * @brief создать контейнер ядра при холодном старте
     * 
     * @param user_factory фабрика пользователей
     * @param group_factory фабрика групп
     * @param obj_factory фабрика объектов файловой системы
     * @param inmem_factory фабрика in-memory файлов
     * @param acl_factory фабрика списков контроля доступа
     * @return std::shared_ptr<IKernelContainer> созданный контейнер ядра
     */
    virtual std::shared_ptr<IKernelContainer> createKernelContainerColdStart(
        std::shared_ptr<IUserFactory> user_factory,
        std::shared_ptr<IGroupFactory> group_factory,
        std::shared_ptr<IFileObjectFactory> obj_factory,
        std::shared_ptr<IInMemoryFactory> inmem_factory,
        std::shared_ptr<IACLFactory> acl_factory
    ) = 0;

    /**
     * @brief создать контейнер ядра из снимка
     * 
     * @param users таблица пользователей
     * @param groups таблица групп
     * @param root_dir корневая директория
     * @return std::shared_ptr<IKernelContainer> созданный контейнер ядра
     */
    virtual std::shared_ptr<IKernelContainer> createKernelContainerFromSnapshot(
        std::unique_ptr<IUserTable> users,
        std::unique_ptr<IGroupTable> groups,
        std::shared_ptr<IFileObject> root_dir
    ) = 0;
};

/**
 * @class KernelFactory
 * 
 * @brief реализация фабрики ядра файловой системы
 * 
 * создает таблицы пользователей и групп, корневую директорию и контейнер ядра с валидацией
 */
class KernelFactory : public IKernelFactory {
public:
    /**
     * @brief конструктор фабрики ядра
     * 
     * @param group_factory фабрика групп
     * @param inmem_factory фабрика in-memory файлов
     * @param user_factory фабрика пользователей
     */
    KernelFactory(std::shared_ptr<domain::IGroupFactory> group_factory,
        std::shared_ptr<domain::IInMemoryFactory> inmem_factory,
        std::shared_ptr<domain::IUserFactory> user_factory) :
    validator_(std::move(group_factory), std::move(inmem_factory), std::move(user_factory)) {}

    /**
     * @brief создать таблицу пользователей с администратором
     * 
     * @param user_factory фабрика пользователей
     * @return std::unique_ptr<IUserTable> созданная таблица пользователей
     */
    std::unique_ptr<IUserTable> createTabUserWithAdmin(std::shared_ptr<IUserFactory> user_factory) override;

    /**
     * @brief создать таблицу групп с группой администраторов
     * 
     * @param group_factory фабрика групп
     * @return std::unique_ptr<IGroupTable> созданная таблица групп
     */
    std::unique_ptr<IGroupTable> createTabGroupWithAdmin(std::shared_ptr<IGroupFactory> group_factory) override;

    /**
     * @brief создать пустую таблицу пользователей
     * 
     * @return std::unique_ptr<IUserTable> созданная таблица пользователей
     */
    std::unique_ptr<IUserTable> createEmptyTabUser() override;

    /**
     * @brief создать пустую таблицу групп
     * 
     * @return std::unique_ptr<IGroupTable> созданная таблица групп
     */
    std::unique_ptr<IGroupTable> createEmptyTabGroup() override;

    /**
     * @brief создать корневую директорию
     * 
     * @param dir_factory фабрика директорий
     * @param inmem_factory фабрика in-memory файлов
     * @param acl_factory фабрика списков контроля доступа
     * @param admin_id идентификатор администратора
     * @param group_admins_id идентификатор группы администраторов
     * @return std::unique_ptr<IFileObject> созданная корневая директория
     */
    std::unique_ptr<IFileObject> createDirRoot(
        std::shared_ptr<IFileObjectFactory> dir_factory,
        std::shared_ptr<IInMemoryFactory> inmem_factory,
        std::shared_ptr<IACLFactory> acl_factory,
        userId_t admin_id,
        groupId_t group_admins_id) override;

    /**
     * @brief создать контейнер ядра при холодном старте
     * 
     * @param user_factory фабрика пользователей
     * @param group_factory фабрика групп
     * @param obj_factory фабрика объектов файловой системы
     * @param inmem_factory фабрика in-memory файлов
     * @param acl_factory фабрика списков контроля доступа
     * @return std::shared_ptr<IKernelContainer> созданный контейнер ядра
     */
    std::shared_ptr<IKernelContainer> createKernelContainerColdStart(
        std::shared_ptr<IUserFactory> user_factory,
        std::shared_ptr<IGroupFactory> group_factory,
        std::shared_ptr<IFileObjectFactory> obj_factory,
        std::shared_ptr<IInMemoryFactory> inmem_factory,
        std::shared_ptr<IACLFactory> acl_factory) override;

    /**
     * @brief создать контейнер ядра из снимка
     * 
     * @param users таблица пользователей
     * @param groups таблица групп
     * @param root_dir корневая директория
     * @return std::shared_ptr<IKernelContainer> созданный контейнер ядра
     */
    std::shared_ptr<IKernelContainer> createKernelContainerFromSnapshot(
        std::unique_ptr<IUserTable> users,
        std::unique_ptr<IGroupTable> groups,
        std::shared_ptr<IFileObject> root_dir) override;

private:
    KernelFactoryValidator validator_;
};

} // namespace domain