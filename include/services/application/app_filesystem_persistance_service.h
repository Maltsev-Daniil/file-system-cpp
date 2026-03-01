#pragma once

#include <string>

#include "app_base.h"
#include "containers/system_container.h"
#include "serializer/iserializer.h"

class IFileSystemMapper;
class FileSystemSnapshot;

namespace domain {
class IKernelFactory;
class IKernelContainer;
}

namespace services {

/**
 * @class IFileSystemPersistenceAppService
 * 
 * @brief интерфейс для сервиса сохранения и загрузки файловой системы
 * 
 * предоставляет операции для сохранения состояния файловой системы в файл и загрузки из файла
 */
class IFileSystemPersistenceAppService {
public:
    /**
     * @brief деструктор интерфейса сервиса персистентности файловой системы
     */
    virtual ~IFileSystemPersistenceAppService() = default;

    /**
     * @brief сохранить файловую систему в файл
     * 
     * @param path путь к файлу для сохранения
     */
    virtual void save(const std::string& path) = 0;

    /**
     * @brief загрузить файловую систему из файла
     * 
     * @param path путь к файлу для загрузки
     */
    virtual void load(const std::string& path) = 0;
};

/**
 * @class FileSystemPersistenceAppService
 * 
 * @brief реализация сервиса сохранения и загрузки файловой системы
 * 
 * координирует работу маппера и сериализатора для сохранения и загрузки состояния файловой системы
 */
class FileSystemPersistenceAppService final
    : public IFileSystemPersistenceAppService, public AppServiceBase {
public:
    /**
     * @brief конструктор сервиса персистентности файловой системы
     * 
     * @param system_container контейнер системных компонентов
     * @param snapshot_mapper маппер для работы со снимками файловой системы
     * @param kernel_factory фабрика ядра файловой системы
     * @param serializer сериализатор для записи и чтения файлов
     * @param session сервис сессии
     */
    FileSystemPersistenceAppService(
        SystemContainer& system_container,
        IFileSystemMapper& snapshot_mapper,
        domain::IKernelFactory& kernel_factory,
        ISerializer<FileSystemSnapshot>& serializer,
        ISessionService& session
    ) :
        AppServiceBase(session),
        system_container_(system_container),
        snapshot_mapper_(snapshot_mapper),
        kernel_factory_(kernel_factory),
        serializer_(serializer) {}

    /**
     * @brief сохранить файловую систему в файл
     * 
     * @param path путь к файлу для сохранения
     */
    void save(const std::string& path) override;

    /**
     * @brief загрузить файловую систему из файла
     * 
     * @param path путь к файлу для загрузки
     */
    void load(const std::string& path) override;

private:
    SystemContainer& system_container_;
    IFileSystemMapper& snapshot_mapper_;
    domain::IKernelFactory& kernel_factory_;
    ISerializer<FileSystemSnapshot>& serializer_;
};

} // namespace services