#pragma once

#include "domain/types.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

namespace domain {
    class IFileObject;
    class IDirectoryStructure;
    class InMemoryFile;
    class IInMemoryFactory;
    class IFileObjectFactory;
    class IACLFactory;
    class IDirectory;
}

namespace services {
    class IKernelService;
}

namespace services {

/**
 * @class IDirectoryService
 * 
 * @brief интерфейс для сервиса работы с директориями
 * 
 * предоставляет операции для создания, переименования и управления содержимым директорий
 */
class IDirectoryService {
public:
    /**
     * @brief деструктор интерфейса сервиса директорий
     */
    virtual ~IDirectoryService() = default;

    /**
     * @brief создать директорию
     * 
     * @param name имя директории
     * @param dir_path путь к родительской директории
     * @param owner_uid идентификатор владельца
     * @param owner_gid идентификатор группы владельца
     * @return std::unique_ptr<domain::IFileObject> созданная директория
     */
    virtual std::unique_ptr<domain::IFileObject> createDirectory(
        const std::string& name,
        const std::string& dir_path,
        domain::userId_t owner_uid,
        domain::groupId_t owner_gid) = 0;

    /**
     * @brief переименовать директорию
     * 
     * @param dir директория для переименования
     * @param new_name новое имя директории
     */
    virtual void renameDirectory(domain::IFileObject* dir,
                                 const std::string& new_name) = 0;
                                 
    /**
     * @brief получить список содержимого директории
     * 
     * @param dir директория
     * @return std::vector<std::string> вектор имен объектов в директории
     */
    virtual std::vector<std::string> listDirectory(domain::IFileObject* dir) = 0;

    /**
     * @brief вставить объект в директорию
     * 
     * @param object объект для вставки
     * @param dir_struct структура директории
     */
    virtual void insertEntry(std::unique_ptr<domain::IFileObject> object,
                             domain::IFileObject* dir_struct) = 0;

    /**
     * @brief удалить объект из директории
     * 
     * @param name имя объекта
     * @param dir_struct структура директории
     */
    virtual void removeEntry(const std::string& name,
                             domain::IFileObject* dir_struct) = 0;

    /**
     * @brief переименовать объект в директории
     * 
     * @param old_name старое имя объекта
     * @param new_name новое имя объекта
     * @param dir_structure структура директории
     */
    virtual void renameEntry(const std::string& old_name,
                             const std::string& new_name,
                             domain::IFileObject* dir_structure) = 0;

    /**
     * @brief найти объект в директории по имени
     * 
     * @param name имя объекта
     * @param dir_structure структура директории
     * @return domain::IFileObject* указатель на объект или nullptr если не найден
     */
    virtual domain::IFileObject* findEntry(const std::string& name,
                                           domain::IFileObject* dir_structure) = 0;

    /**
     * @brief вычислить размер директории
     * 
     * @param dir директория
     * @return size_t размер директории в байтах
     */
    virtual size_t calculateDirectorySize(domain::IDirectory* dir) = 0;

    /**
     * @brief обновить размер директории рекурсивно
     * 
     * @param dir директория
     * @param kernel сервис ядра файловой системы
     */
    virtual void updateDirectorySizeRecursive(domain::IDirectory* dir, IKernelService* kernel) = 0;
};


/**
 * @class DirectoryService
 * 
 * @brief реализация сервиса работы с директориями
 * 
 * выполняет операции для создания, переименования и управления содержимым директорий
 */
class DirectoryService : public IDirectoryService {
private:
    std::shared_ptr<domain::IFileObjectFactory> dir_fac;
    std::shared_ptr<domain::IInMemoryFactory> inmem_fac;
    std::shared_ptr<domain::IACLFactory> acl_fac;

public:
    /**
     * @brief конструктор сервиса директорий
     * 
     * @param df фабрика объектов файловой системы
     * @param imf фабрика in-memory файлов
     * @param af фабрика списков контроля доступа
     */
    DirectoryService(std::shared_ptr<domain::IFileObjectFactory> df,
                     std::shared_ptr<domain::IInMemoryFactory> imf,
                     std::shared_ptr<domain::IACLFactory> af);

    std::unique_ptr<domain::IFileObject> createDirectory(
        const std::string& name,
        const std::string& dir_path,
        domain::userId_t owner_uid,
        domain::groupId_t owner_gid) override;

    void renameDirectory(domain::IFileObject* dir,
                         const std::string& new_name) override;

    std::vector<std::string> listDirectory(domain::IFileObject* dir) override;

    void insertEntry(std::unique_ptr<domain::IFileObject> object,
                     domain::IFileObject* dir_struct) override;

    void removeEntry(const std::string& name,
                     domain::IFileObject* dir_struct) override;

    void renameEntry(const std::string& old_name,
                     const std::string& new_name,
                     domain::IFileObject* dir_structure) override;

    domain::IFileObject* findEntry(const std::string& name,
                                   domain::IFileObject* dir_structure) override;

    size_t calculateDirectorySize(domain::IDirectory* dir) override;

    void updateDirectorySizeRecursive(domain::IDirectory* dir, IKernelService* kernel) override;
};

}