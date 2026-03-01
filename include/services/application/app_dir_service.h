#pragma once

#include <string>
#include <memory>

#include "app_base.h"
#include "services/session/session_service.h"

struct DirContentDTO;

namespace services {

class IRightsService;
class IKernelService;
class IDirectoryService;

/**
 * @class IDirectoryAppService
 * 
 * @brief интерфейс для сервиса работы с директориями на уровне приложения
 * 
 * предоставляет операции для создания и просмотра директорий
 */
class IDirectoryAppService {
public:
    /**
     * @brief деструктор интерфейса сервиса директорий
     */
    virtual ~IDirectoryAppService() = default;

    /**
     * @brief создать директорию
     * 
     * @param dirname имя директории
     * @param dir_path путь к родительской директории
     */
    virtual void createDirectory(const std::string& dirname, const std::string& dir_path) = 0;

    /**
     * @brief получить содержимое директории
     * 
     * @param path путь к директории
     * @return DirContentDTO данные о содержимом директории
     */
    virtual DirContentDTO listDirectory(const std::string& path) = 0;
};

/**
 * @class DirectoryAppService
 * 
 * @brief реализация сервиса работы с директориями на уровне приложения
 * 
 * координирует работу сервисов для выполнения операций с директориями
 */
class DirectoryAppService : public IDirectoryAppService, public AppServiceBase {
private:
    std::shared_ptr<IRightsService> rights;
    std::shared_ptr<IDirectoryService> dirs;
    std::shared_ptr<IKernelService> kernel;
public:
    /**
     * @brief конструктор сервиса директорий
     * 
     * @param rights сервис прав доступа
     * @param dirs сервис директорий
     * @param kernel сервис ядра файловой системы
     * @param session сервис сессии
     */
    DirectoryAppService(
    std::shared_ptr<IRightsService> rights,
    std::shared_ptr<IDirectoryService> dirs,
    std::shared_ptr<IKernelService> kernel,
    ISessionService& session) :
        AppServiceBase(session),
        rights(std::move(rights)),
        dirs(std::move(dirs)),
        kernel(std::move(kernel)) {}

    /**
     * @brief создать директорию
     * 
     * @param dirname имя директории
     * @param dir_path путь к родительской директории
     */
    void createDirectory(const std::string& dirname, const std::string& dir_path) override;

    /**
     * @brief получить содержимое директории
     * 
     * @param path путь к директории
     * @return DirContentDTO данные о содержимом директории
     */
    DirContentDTO listDirectory(const std::string& path) override;
};
}