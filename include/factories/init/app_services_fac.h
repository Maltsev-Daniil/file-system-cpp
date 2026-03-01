#pragma once

#include <memory>

#include "services/session/session_service.h"

namespace services {
class IFileSystemPersistenceAppService;
}

namespace services {

class IFileAppService;
class IDirectoryAppService;
class IRightsAppService;
class IDirNFileAppService;
class IFileSystemAppService;
class IUserAppService;
class IGroupAppService;

class IFileService;
class IDirectoryService;
class IUserService;
class IGroupService;
class IRightsService;
class IKernelService;

/**
 * @class IAppFactory
 * 
 * @brief интерфейс для фабрики сервисов приложения
 * 
 * предоставляет методы для создания сервисов уровня приложения
 */
class IAppFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики сервисов приложения
     */
    virtual ~IAppFactory() = default;

    /**
     * @brief создать сервис файлов приложения
     * 
     * @return std::shared_ptr<IFileAppService> созданный сервис файлов приложения
     */
    virtual std::shared_ptr<IFileAppService> createFileAppService() = 0;

    /**
     * @brief создать сервис директорий приложения
     * 
     * @return std::shared_ptr<IDirectoryAppService> созданный сервис директорий приложения
     */
    virtual std::shared_ptr<IDirectoryAppService> createDirectoryAppService() = 0;

    /**
     * @brief создать сервис файлов и директорий приложения
     * 
     * @return std::shared_ptr<IDirNFileAppService> созданный сервис файлов и директорий приложения
     */
    virtual std::shared_ptr<IDirNFileAppService> createDirNFileAppService() = 0;

    /**
     * @brief создать сервис файловой системы приложения
     * 
     * @return std::shared_ptr<IFileSystemAppService> созданный сервис файловой системы приложения
     */
    virtual std::shared_ptr<IFileSystemAppService> createFileSystemAppService() = 0;

    /**
     * @brief создать сервис пользователей приложения
     * 
     * @return std::shared_ptr<IUserAppService> созданный сервис пользователей приложения
     */
    virtual std::shared_ptr<IUserAppService> createUserAppService() = 0;

    /**
     * @brief создать сервис групп приложения
     * 
     * @return std::shared_ptr<IGroupAppService> созданный сервис групп приложения
     */
    virtual std::shared_ptr<IGroupAppService> createGroupAppService() = 0;

    /**
     * @brief создать сервис прав доступа приложения
     * 
     * @return std::shared_ptr<IRightsAppService> созданный сервис прав доступа приложения
     */
    virtual std::shared_ptr<IRightsAppService> createRightsAppService() = 0;
};

/**
 * @class AppFactory
 * 
 * @brief реализация фабрики сервисов приложения
 * 
 * создает сервисы уровня приложения с использованием доменных сервисов и сервиса сессии
 */
class AppFactory final : public IAppFactory {
public:
    /**
     * @brief конструктор фабрики сервисов приложения
     * 
     * @param kernel сервис ядра файловой системы
     * @param rights сервис прав доступа
     * @param files сервис файлов
     * @param dirs сервис директорий
     * @param users сервис пользователей
     * @param groups сервис групп
     * @param session сервис сессии
     */
    AppFactory(
        std::shared_ptr<IKernelService> kernel,
        std::shared_ptr<IRightsService> rights,
        std::shared_ptr<IFileService> files,
        std::shared_ptr<IDirectoryService> dirs,
        std::shared_ptr<IUserService> users,
        std::shared_ptr<IGroupService> groups,
        ISessionService& session
    );

    /**
     * @brief создать сервис файлов приложения
     * 
     * @return std::shared_ptr<IFileAppService> созданный сервис файлов приложения
     */
    std::shared_ptr<IFileAppService> createFileAppService() override;

    /**
     * @brief создать сервис директорий приложения
     * 
     * @return std::shared_ptr<IDirectoryAppService> созданный сервис директорий приложения
     */
    std::shared_ptr<IDirectoryAppService> createDirectoryAppService() override;

    /**
     * @brief создать сервис файлов и директорий приложения
     * 
     * @return std::shared_ptr<IDirNFileAppService> созданный сервис файлов и директорий приложения
     */
    std::shared_ptr<IDirNFileAppService> createDirNFileAppService() override;

    /**
     * @brief создать сервис файловой системы приложения
     * 
     * @return std::shared_ptr<IFileSystemAppService> созданный сервис файловой системы приложения
     */
    std::shared_ptr<IFileSystemAppService> createFileSystemAppService() override;

    /**
     * @brief создать сервис пользователей приложения
     * 
     * @return std::shared_ptr<IUserAppService> созданный сервис пользователей приложения
     */
    std::shared_ptr<IUserAppService> createUserAppService() override;

    /**
     * @brief создать сервис групп приложения
     * 
     * @return std::shared_ptr<IGroupAppService> созданный сервис групп приложения
     */
    std::shared_ptr<IGroupAppService> createGroupAppService() override;

    /**
     * @brief создать сервис прав доступа приложения
     * 
     * @return std::shared_ptr<IRightsAppService> созданный сервис прав доступа приложения
     */
    std::shared_ptr<IRightsAppService> createRightsAppService() override;

private:
    std::shared_ptr<IKernelService> kernel;
    std::shared_ptr<IRightsService> rights;
    std::shared_ptr<IFileService> files;
    std::shared_ptr<IDirectoryService> dirs;
    std::shared_ptr<IUserService> users;
    std::shared_ptr<IGroupService> groups;
    ISessionService& session;
};

} // namespace services