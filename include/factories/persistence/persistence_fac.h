#pragma once
#include <memory>

class IPersistenceContainer;

namespace domain {
class IKernelFactory;
}

class SystemContainer;

namespace services {
class ISessionService;
class IFileSystemPersistenceAppService;

/**
 * @class IPersistenceServiceFactory
 * 
 * @brief интерфейс для фабрики сервисов персистентности
 * 
 * предоставляет метод для создания сервиса персистентности файловой системы
 */
class IPersistenceServiceFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики сервисов персистентности
     */
    virtual ~IPersistenceServiceFactory() = default;

    /**
     * @brief создать сервис персистентности файловой системы
     * 
     * @return std::unique_ptr<services::IFileSystemPersistenceAppService> созданный сервис персистентности
     */
    virtual std::unique_ptr<services::IFileSystemPersistenceAppService>
    createFileSystemPersistenceService() = 0;
};

/**
 * @class PersistenceServiceFactory
 * 
 * @brief реализация фабрики сервисов персистентности
 * 
 * создает сервис персистентности файловой системы с использованием системного контейнера и маппера
 */
class PersistenceServiceFactory final
    : public IPersistenceServiceFactory
{
public:
    /**
     * @brief конструктор фабрики сервисов персистентности
     * 
     * @param system_container системный контейнер
     * @param kernel_factory фабрика ядра файловой системы
     * @param session_service сервис сессии
     * @param persistence_container контейнер персистентности
     */
    PersistenceServiceFactory(SystemContainer& system_container,
        domain::IKernelFactory& kernel_factory,
        services::ISessionService& session_service,
        IPersistenceContainer& persistence_container
    );

    /**
     * @brief создать сервис персистентности файловой системы
     * 
     * @return std::unique_ptr<services::IFileSystemPersistenceAppService> созданный сервис персистентности
     */
    std::unique_ptr<services::IFileSystemPersistenceAppService>
    createFileSystemPersistenceService() override;


private:
    SystemContainer& system_container;
    domain::IKernelFactory& kernel_factory;
    services::ISessionService& session_service;
    IPersistenceContainer& persistence_container;
};
}