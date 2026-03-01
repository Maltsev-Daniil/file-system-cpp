#pragma once

#include <memory>

#include "cmd_dispatcher.h"
#include "io.h"
#include "services/shell/path_pattern_resolver.h"
#include "containers/persistence_container.h"
#include "factories/persistence/persistence_fac.h"


namespace domain {
class IInMemoryFactory;
class IACLFactory;
class IKernelFactory;
class IKernelContainer;
}

namespace services {
class IFileSystemPersistenceAppService;
class IKernelService;
class IFileService;
class IDirectoryService;
class IUserService;
class IGroupService;
class IRightsService;

class IFileAppService;
class IDirectoryAppService;
class IDirNFileAppService;
class IFileSystemAppService;
class IUserAppService;
class IGroupAppService;
class IRightsAppService;
class IPersistenceServiceFactory;

class IAppFactory;
class IDomainServicesFactory;

class IPathPatternResolver;
}

class ICmdletFactory;

/**
 * @class SystemContainer
 * 
 * @brief контейнер системных компонентов
 * 
 * централизованное хранилище всех сервисов, фабрик и компонентов системы
 */
class SystemContainer final {
public:
    /**
     * @brief конструктор системного контейнера
     * 
     * @param kernel_container контейнер ядра файловой системы
     * @param domain_factory фабрика доменных сервисов
     * @param dispatcher диспетчер команд
     * @param output вывод для результатов
     * @param kernel_factory фабрика ядра файловой системы
     * @param acl_factory фабрика списков контроля доступа
     * @param inmem_factory фабрика in-memory файлов
     */
    SystemContainer(
        std::shared_ptr<domain::IKernelContainer> kernel_container,
        std::unique_ptr<services::IDomainServicesFactory> domain_factory,
        std::shared_ptr<ICmdDispatcher> dispatcher,
        IOutput &output,
        std::unique_ptr<domain::IKernelFactory> kernel_factory,
        std::shared_ptr<domain::IACLFactory> acl_factory,
        std::shared_ptr<domain::IInMemoryFactory> inmem_factory
        );

    /**
     * @brief получить контейнер ядра файловой системы
     * 
     * @return domain::IKernelContainer* указатель на контейнер ядра
     */
    domain::IKernelContainer *getKernelContainer() const noexcept {
        return kernel_container.get();
    }

    /**
     * @brief получить сервис ядра файловой системы
     * 
     * @return services::IKernelService* указатель на сервис ядра
     */
    services::IKernelService *getKernelService() const;

    /**
     * @brief получить сервис файлов
     * 
     * @return services::IFileService* указатель на сервис файлов
     */
    services::IFileService *getFileService() const;

    /**
     * @brief получить сервис директорий
     * 
     * @return services::IDirectoryService* указатель на сервис директорий
     */
    services::IDirectoryService *getDirectoryService() const;

    /**
     * @brief получить сервис пользователей
     * 
     * @return services::IUserService* указатель на сервис пользователей
     */
    services::IUserService *getUserService() const;

    /**
     * @brief получить сервис групп
     * 
     * @return services::IGroupService* указатель на сервис групп
     */
    services::IGroupService *getGroupService() const;

    /**
     * @brief получить сервис прав доступа
     * 
     * @return services::IRightsService* указатель на сервис прав доступа
     */
    services::IRightsService *getRightsService() const;

    /**
     * @brief получить сервис файлов на уровне приложения
     * 
     * @return services::IFileAppService* указатель на сервис файлов приложения
     */
    services::IFileAppService *getFileAppService() const;

    /**
     * @brief получить сервис директорий на уровне приложения
     * 
     * @return services::IDirectoryAppService* указатель на сервис директорий приложения
     */
    services::IDirectoryAppService *getDirectoryAppService() const;

    /**
     * @brief получить сервис файлов и директорий на уровне приложения
     * 
     * @return services::IDirNFileAppService* указатель на сервис файлов и директорий приложения
     */
    services::IDirNFileAppService *getDirNFileAppService() const;

    /**
     * @brief получить сервис файловой системы на уровне приложения
     * 
     * @return services::IFileSystemAppService* указатель на сервис файловой системы приложения
     */
    services::IFileSystemAppService *getFileSystemAppService() const;

    /**
     * @brief получить сервис персистентности файловой системы
     * 
     * @return services::IFileSystemPersistenceAppService* указатель на сервис персистентности
     */
    services::IFileSystemPersistenceAppService *getFileSystemPersistenceAppService() const;

    /**
     * @brief получить сервис пользователей на уровне приложения
     * 
     * @return services::IUserAppService* указатель на сервис пользователей приложения
     */
    services::IUserAppService *getUserAppService() const;

    /**
     * @brief получить сервис групп на уровне приложения
     * 
     * @return services::IGroupAppService* указатель на сервис групп приложения
     */
    services::IGroupAppService *getGroupAppService() const;

    /**
     * @brief получить сервис прав доступа на уровне приложения
     * 
     * @return services::IRightsAppService* указатель на сервис прав доступа приложения
     */
    services::IRightsAppService *getRightsAppService() const;

    /**
     * @brief заменить контейнер ядра файловой системы
     * 
     * @param kernel_container новый контейнер ядра
     */
    void replaceKernelContainer(std::shared_ptr<domain::IKernelContainer>
        kernel_container);

    /**
     * @brief инициализировать системный контейнер
     */
    void init();

private:
    /**
     * @brief инициализировать сервисы
     */
    void initServices();

    /**
     * @brief очистить сервисы
     */
    void cleanupServices();
    std::shared_ptr<domain::IKernelContainer> kernel_container;
    std::unique_ptr<IPersistenceContainer> persistence_container;

    std::unique_ptr<services::IDomainServicesFactory> domain_factory;
    std::unique_ptr<services::IAppFactory> app_factory;
    std::unique_ptr<services::IPersistenceServiceFactory> persistence_factory;
    std::unique_ptr<ICmdletFactory> cmdlet_factory;

    // for persistence
    std::unique_ptr<domain::IKernelFactory> kernel_factory;
    std::shared_ptr<domain::IACLFactory> acl_factory;
    std::shared_ptr<domain::IInMemoryFactory> inmem_factory;

    std::shared_ptr<services::IKernelService> kernel_service;
    std::shared_ptr<services::IFileService> file_service;
    std::shared_ptr<services::IDirectoryService> dir_service;
    std::shared_ptr<services::IUserService> user_service;
    std::shared_ptr<services::IGroupService> group_service;
    std::shared_ptr<services::IRightsService> rights_service;
    std::shared_ptr<services::ISessionService> session_service;

    std::shared_ptr<services::IFileAppService> file_app;
    std::shared_ptr<services::IDirectoryAppService> dir_app;
    std::shared_ptr<services::IDirNFileAppService> dir_n_file_app;
    std::shared_ptr<services::IFileSystemAppService> fs_app;
    std::shared_ptr<services::IFileSystemPersistenceAppService> fs_persistence_app;
    std::shared_ptr<services::IUserAppService> user_app;
    std::shared_ptr<services::IGroupAppService> group_app;
    std::shared_ptr<services::IRightsAppService> rights_app;

    std::shared_ptr<ICmdDispatcher> dispatcher;
    std::shared_ptr<services::IPathPatternResolver> path_resolver;
    IOutput &output;
};
