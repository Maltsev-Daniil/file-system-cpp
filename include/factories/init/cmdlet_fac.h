#pragma once

#include <memory>

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
class IPathPatternResolver;
}

class ICmdDispatcher;

class IOutput;

/**
 * @class ICmdletFactory
 * 
 * @brief интерфейс для фабрики команд
 * 
 * предоставляет метод для регистрации команд в диспетчере
 */
class ICmdletFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики команд
     */
    virtual ~ICmdletFactory() = default;

    /**
     * @brief зарегистрировать все команды в диспетчере
     */
    virtual void registerCmdlets() = 0;
};

/**
 * @class CmdletFactory
 * 
 * @brief реализация фабрики команд
 * 
 * создает команды и регистрирует их в диспетчере команд
 */
class CmdletFactory final : public ICmdletFactory {
public:
    /**
     * @brief конструктор фабрики команд
     * 
     * @param file_app сервис файлов приложения
     * @param dir_app сервис директорий приложения
     * @param right_app сервис прав доступа приложения
     * @param dir_and_file_app сервис файлов и директорий приложения
     * @param file_system_app сервис файловой системы приложения
     * @param file_system_persistence_app сервис персистентности файловой системы
     * @param user_app сервис пользователей приложения
     * @param group_app сервис групп приложения
     * @param path_resolver резолвер путей
     * @param dispatcher диспетчер команд
     * @param output вывод для результатов
     */
    CmdletFactory(
        std::shared_ptr<services::IFileAppService> file_app,
        std::shared_ptr<services::IDirectoryAppService> dir_app,
        std::shared_ptr<services::IRightsAppService> right_app,
        std::shared_ptr<services::IDirNFileAppService> dir_and_file_app,
        std::shared_ptr<services::IFileSystemAppService> file_system_app,
        std::shared_ptr<services::IFileSystemPersistenceAppService> file_system_persistence_app,
        std::shared_ptr<services::IUserAppService> user_app,
        std::shared_ptr<services::IGroupAppService> group_app,
        std::shared_ptr<services::IPathPatternResolver> path_resolver,
        std::shared_ptr<ICmdDispatcher> dispatcher,
        IOutput& output
    );

    /**
     * @brief зарегистрировать все команды в диспетчере
     */
    void registerCmdlets() override;

private:
    /**
     * @brief зарегистрировать команды для работы с файлами
     */
    void registerFileCmdlets();

    /**
     * @brief зарегистрировать команды для работы с директориями
     */
    void registerDirectoryCmdlets();

    /**
     * @brief зарегистрировать команды для работы с правами доступа
     */
    void registerRightsCmdlets();

    /**
     * @brief зарегистрировать команды для работы с файлами и директориями
     */
    void registerDirNFileCmdlets();

    /**
     * @brief зарегистрировать команды для работы с файловой системой
     */
    void registerFileSystemCmdlets();

    /**
     * @brief зарегистрировать команды для работы с пользователями
     */
    void registerUserCmdlets();

    /**
     * @brief зарегистрировать команды для работы с группами
     */
    void registerGroupCmdlets();

    std::shared_ptr<services::IFileAppService> file_app;
    std::shared_ptr<services::IDirectoryAppService> dir_app;
    std::shared_ptr<services::IRightsAppService> right_app;
    std::shared_ptr<services::IDirNFileAppService> dir_and_file_app;
    std::shared_ptr<services::IFileSystemAppService> file_system_app;
    std::shared_ptr<services::IFileSystemPersistenceAppService> file_system_persistence_app;
    std::shared_ptr<services::IUserAppService> user_app;
    std::shared_ptr<services::IGroupAppService> group_app;
    std::shared_ptr<services::IPathPatternResolver> path_resolver;
    std::shared_ptr<ICmdDispatcher> dispatcher;
    IOutput& output;
};