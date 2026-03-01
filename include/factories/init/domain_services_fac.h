#pragma once

#include <memory>

#include "domain/tables/itable.h"

namespace services {
class IFileSystemPersistenceAppService;
class ISessionService;
}

namespace domain {
    class IFileObjectFactory;
    class IUserFactory;
    class IGroupFactory;
    class IInMemoryFactory;
    class IACLFactory;

    class IKernelContainer;
}

namespace services {

class IFileService;
class IDirectoryService;
class IUserService;
class IGroupService;
class IRightsService;
class IKernelService;

/**
 * @class IDomainServicesFactory
 * 
 * @brief интерфейс для фабрики доменных сервисов
 * 
 * предоставляет методы для создания сервисов доменного слоя
 */
class IDomainServicesFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики доменных сервисов
     */
    virtual ~IDomainServicesFactory() = default;

    /**
     * @brief создать сервис файлов
     * 
     * @return std::shared_ptr<IFileService> созданный сервис файлов
     */
    virtual std::shared_ptr<IFileService> createFileService() = 0;

    /**
     * @brief создать сервис директорий
     * 
     * @return std::shared_ptr<IDirectoryService> созданный сервис директорий
     */
    virtual std::shared_ptr<IDirectoryService> createDirectoryService() = 0;

    /**
     * @brief создать сервис пользователей
     * 
     * @return std::shared_ptr<IUserService> созданный сервис пользователей
     */
    virtual std::shared_ptr<IUserService> createUserService() = 0;

    /**
     * @brief создать сервис групп
     * 
     * @return std::shared_ptr<IGroupService> созданный сервис групп
     */
    virtual std::shared_ptr<IGroupService> createGroupService() = 0;

    /**
     * @brief создать сервис прав доступа
     * 
     * @param user_service сервис пользователей
     * @param group_service сервис групп
     * @return std::shared_ptr<IRightsService> созданный сервис прав доступа
     */
    virtual std::shared_ptr<IRightsService> createRightsService(
        std::shared_ptr<IUserService> user_service,
        std::shared_ptr<IGroupService> group_service
    ) = 0;

    /**
     * @brief создать сервис ядра файловой системы
     * 
     * @return std::shared_ptr<IKernelService> созданный сервис ядра
     */
    virtual std::shared_ptr<IKernelService> createKernelService() = 0;

    /**
     * @brief создать сервис сессии
     * 
     * @return std::shared_ptr<ISessionService> созданный сервис сессии
     */
    virtual std::shared_ptr<ISessionService> createSessionService() = 0;
};

/**
 * @class DomainServicesFactory
 * 
 * @brief реализация фабрики доменных сервисов
 * 
 * создает сервисы доменного слоя с использованием фабрик объектов и контейнера ядра
 */
class DomainServicesFactory final : public IDomainServicesFactory {
public:
    /**
     * @brief конструктор фабрики доменных сервисов
     * 
     * @param file_factory фабрика файлов
     * @param dir_factory фабрика директорий
     * @param user_factory фабрика пользователей
     * @param group_factory фабрика групп
     * @param inmem_factory фабрика in-memory файлов
     * @param acl_factory фабрика списков контроля доступа
     * @param kernel_container контейнер ядра файловой системы
     */
    DomainServicesFactory(
        std::shared_ptr<domain::IFileObjectFactory> file_factory,
        std::shared_ptr<domain::IFileObjectFactory> dir_factory,
        std::shared_ptr<domain::IUserFactory> user_factory,
        std::shared_ptr<domain::IGroupFactory> group_factory,
        std::shared_ptr<domain::IInMemoryFactory> inmem_factory,
        std::shared_ptr<domain::IACLFactory> acl_factory,
        std::shared_ptr<domain::IKernelContainer> kernel_container
    );

    /**
     * @brief создать сервис файлов
     * 
     * @return std::shared_ptr<IFileService> созданный сервис файлов
     */
    std::shared_ptr<IFileService> createFileService() override;

    /**
     * @brief создать сервис директорий
     * 
     * @return std::shared_ptr<IDirectoryService> созданный сервис директорий
     */
    std::shared_ptr<IDirectoryService> createDirectoryService() override;

    /**
     * @brief создать сервис пользователей
     * 
     * @return std::shared_ptr<IUserService> созданный сервис пользователей
     */
    std::shared_ptr<IUserService> createUserService() override;

    /**
     * @brief создать сервис групп
     * 
     * @return std::shared_ptr<IGroupService> созданный сервис групп
     */
    std::shared_ptr<IGroupService> createGroupService() override;

    /**
     * @brief создать сервис прав доступа
     * 
     * @param user_service сервис пользователей
     * @param group_service сервис групп
     * @return std::shared_ptr<IRightsService> созданный сервис прав доступа
     */
    std::shared_ptr<IRightsService> createRightsService(
        std::shared_ptr<IUserService> user_service,
        std::shared_ptr<IGroupService> group_service
    ) override;

    /**
     * @brief создать сервис ядра файловой системы
     * 
     * @return std::shared_ptr<IKernelService> созданный сервис ядра
     */
    std::shared_ptr<IKernelService> createKernelService() override;

    /**
     * @brief создать сервис сессии
     * 
     * @return std::shared_ptr<ISessionService> созданный сервис сессии
     */
    std::shared_ptr<ISessionService> createSessionService() override;

    /**
     * @brief обновить контейнер ядра файловой системы
     * 
     * @param new_container новый контейнер ядра
     */
    void updateKernelContainer(std::shared_ptr<domain::IKernelContainer> new_container);

private:
    std::shared_ptr<domain::IFileObjectFactory> file_factory;
    std::shared_ptr<domain::IFileObjectFactory> dir_factory;
    std::shared_ptr<domain::IUserFactory> user_factory;
    std::shared_ptr<domain::IGroupFactory> group_factory;
    std::shared_ptr<domain::IInMemoryFactory> inmem_factory;
    std::shared_ptr<domain::IACLFactory> acl_factory;

    std::shared_ptr<domain::IKernelContainer> kernel_container;
};

} // namespace services