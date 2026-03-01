#pragma once

#include <string>
#include <memory>

#include "app_base.h"
#include "services/session/session_service.h"

struct StatFileDTO;

namespace services {

class IRightsService;
class IKernelService;
class IDirectoryService;
class IFileService;

/**
 * @class IDirNFileAppService
 * 
 * @brief интерфейс для сервиса работы с файлами и директориями на уровне приложения
 * 
 * предоставляет операции для работы как с файлами, так и с директориями
 */
class IDirNFileAppService {
    public:
    /**
     * @brief деструктор интерфейса сервиса файлов и директорий
     */
    virtual ~IDirNFileAppService() = default;

    /**
     * @brief удалить файл или директорию
     * 
     * @param path путь к файлу или директории
     */
    virtual void remove(const std::string& path) = 0;

    /**
     * @brief получить статистику файла или директории
     * 
     * @param path путь к файлу или директории
     * @return StatFileDTO данные о статистике
     */
    virtual StatFileDTO getStat(const std::string& path) = 0;

    /**
     * @brief переместить файл или директорию
     * 
     * @param path путь к файлу или директории
     * @param target_dir_path путь к целевой директории
     */
    virtual void move(const std::string& path, const std::string& target_dir_path) = 0;

    /**
     * @brief скопировать файл или директорию
     * 
     * @param path путь к файлу или директории
     * @param target_dir_path путь к целевой директории
     */
    virtual void copy(const std::string& path, const std::string& target_dir_path) = 0;
};

/**
 * @class DirNFileAppService
 * 
 * @brief реализация сервиса работы с файлами и директориями на уровне приложения
 * 
 * координирует работу сервисов для выполнения операций с файлами и директориями
 */
class DirNFileAppService : public IDirNFileAppService, public AppServiceBase {
    std::shared_ptr<IRightsService> rights;
    std::shared_ptr<IDirectoryService> dirs;
    std::shared_ptr<IKernelService> kernel;
public:
    /**
     * @brief конструктор сервиса файлов и директорий
     * 
     * @param rights сервис прав доступа
     * @param dirs сервис директорий
     * @param kernel сервис ядра файловой системы
     * @param session сервис сессии
     */
    DirNFileAppService (
        std::shared_ptr<IRightsService> rights,
        std::shared_ptr<IDirectoryService> dirs,
        std::shared_ptr<IKernelService> kernel,
        ISessionService& session
    ) :
    AppServiceBase(session),
    rights(std::move(rights)),
    dirs(std::move(dirs)),
    kernel(std::move(kernel)) {}

    /**
     * @brief удалить файл или директорию
     * 
     * @param path путь к файлу или директории
     */
    void remove(const std::string& path) override;

    /**
     * @brief получить статистику файла или директории
     * 
     * @param path путь к файлу или директории
     * @return StatFileDTO данные о статистике
     */
    StatFileDTO getStat(const std::string& path) override;

    /**
     * @brief переместить файл или директорию
     * 
     * @param path путь к файлу или директории
     * @param target_dir_path путь к целевой директории
     */
    void move(const std::string& path, const std::string& target_dir_path) override;

    /**
     * @brief скопировать файл или директорию
     * 
     * @param path путь к файлу или директории
     * @param target_dir_path путь к целевой директории
     */
    void copy(const std::string& path, const std::string& target_dir_path) override;
};

}