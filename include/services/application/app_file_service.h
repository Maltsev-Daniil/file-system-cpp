#pragma once

#include <memory>
#include <string>

#include "app_base.h"

struct FileContentDTO;

namespace services {

class IRightsService;
class IFileService;
class IKernelService;
class IDirectoryService;
/**
 * @class IFileAppService
 * 
 * @brief интерфейс для сервиса работы с файлами на уровне приложения
 * 
 * предоставляет операции для чтения, записи и создания файлов
 */
class IFileAppService {

public:
    /**
     * @brief деструктор интерфейса сервиса файлов
     */
    virtual ~IFileAppService() = default;

    /**
     * @brief прочитать содержимое файла
     * 
     * @param path путь к файлу
     * @return FileContentDTO данные о содержимом файла
     */
    virtual FileContentDTO readFile(const std::string& path) = 0;

    /**
     * @brief записать данные в файл
     * 
     * @param path путь к файлу
     * @param data данные для записи
     */
    virtual void writeFile(const std::string& path,
                           const std::string& data) = 0;

    /**
     * @brief добавить данные в конец файла
     * 
     * @param path путь к файлу
     * @param data данные для добавления
     */
    virtual void appendFile(const std::string& path, const std::string& data) = 0;

    /**
     * @brief создать новый файл
     * 
     * @param filename имя файла
     * @param path путь к директории
     */
    virtual void createFile(const std::string& filename, const std::string& path) = 0;
};

/**
 * @class FileAppService
 * 
 * @brief реализация сервиса работы с файлами на уровне приложения
 * 
 * координирует работу сервисов для выполнения операций с файлами
 */
class FileAppService : public IFileAppService, public AppServiceBase {
private:
    std::shared_ptr<IRightsService> rights;
    std::shared_ptr<IFileService> files;
    std::shared_ptr<IDirectoryService> dirs;
    std::shared_ptr<IKernelService> kernel;

public:
    /**
     * @brief конструктор сервиса файлов
     * 
     * @param rights сервис прав доступа
     * @param files сервис файлов
     * @param dirs сервис директорий
     * @param kernel сервис ядра файловой системы
     * @param session сервис сессии
     */
    FileAppService(
        std::shared_ptr<IRightsService> rights,
        std::shared_ptr<IFileService> files,
        std::shared_ptr<IDirectoryService> dirs,
        std::shared_ptr<IKernelService> kernel,
        ISessionService& session
    ) :
    AppServiceBase(session),
    rights(std::move(rights)),
    files(std::move(files)),
    dirs(std::move(dirs)),
    kernel(std::move(kernel)) {}

    /**
     * @brief прочитать содержимое файла
     * 
     * @param path путь к файлу
     * @return FileContentDTO данные о содержимом файла
     */
    FileContentDTO readFile(const std::string& path) override;
 
    /**
     * @brief записать данные в файл
     * 
     * @param path путь к файлу
     * @param data данные для записи
     */
    void writeFile(const std::string& path,
                   const std::string& data) override;

    /**
     * @brief добавить данные в конец файла
     * 
     * @param path путь к файлу
     * @param data данные для добавления
     */
    void appendFile(const std::string& path, const std::string& data) override;

    /**
     * @brief создать новый файл
     * 
     * @param filename имя файла
     * @param path путь к директории
     */
    void createFile(const std::string& filename, const std::string& path) override;
};

}