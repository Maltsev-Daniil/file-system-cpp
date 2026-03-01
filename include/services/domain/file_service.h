#pragma once

#include "domain/types.h"
#include <memory>
#include <string>
#include <stdexcept>

namespace domain {
class IFileObjectFactory;
}

namespace domain {
class IFileObject;
class IInMemoryFile;
class IFileFactory;
class IInMemoryFactory;
class IACLFactory;
}

namespace services {

/**
 * @class IFileService
 * 
 * @brief интерфейс для сервиса работы с файлами
 * 
 * предоставляет операции для создания, чтения, записи и управления файлами
 */
class IFileService {
public:
    /**
     * @brief деструктор интерфейса сервиса файлов
     */
    virtual ~IFileService() = default;

    /**
     * @brief создать файл
     * 
     * @param dir_path путь к директории
     * @param file_name имя файла
     * @param owner_uid идентификатор владельца
     * @param owner_gid идентификатор группы владельца
     * @return std::unique_ptr<domain::IFileObject> созданный файл
     */
    virtual std::unique_ptr<domain::IFileObject> createFile(
        const std::string& dir_path,
        const std::string& file_name,
        domain::userId_t owner_uid,
        domain::groupId_t owner_gid) = 0;

    /**
     * @brief открыть файл для чтения
     * 
     * @param file файл для открытия
     */
    virtual void openFileForRead(domain::IFileObject* file) = 0;

    /**
     * @brief открыть файл для записи
     * 
     * @param file файл для открытия
     */
    virtual void openFileForWrite(domain::IFileObject* file) = 0;

    /**
     * @brief закрыть файл
     * 
     * @param file файл для закрытия
     */
    virtual void closeFile(domain::IFileObject* file) = 0;

    /**
     * @brief прочитать содержимое файла
     * 
     * @param file файл для чтения
     * @return const std::string& содержимое файла
     */
    virtual const std::string& readFile(domain::IFileObject* file) = 0;

    /**
     * @brief записать данные в файл
     * 
     * @param file файл для записи
     * @param data данные для записи
     */
    virtual void writeFile(domain::IFileObject* file, const std::string& data) = 0;

    /**
     * @brief добавить данные в конец файла
     * 
     * @param file файл для добавления
     * @param data данные для добавления
     */
    virtual void appendToFile(domain::IFileObject* file, const std::string& data) = 0;

    /**
     * @brief получить информацию о файле
     * 
     * @param file файл
     * @return domain::IInMemoryFile* указатель на in-memory представление файла
     */
    virtual domain::IInMemoryFile* getFileInfo(domain::IFileObject* file) = 0;

    /**
     * @brief переименовать файл
     * 
     * @param file файл для переименования
     * @param new_name новое имя файла
     */
    virtual void renameFile(domain::IFileObject* file, const std::string& new_name) = 0;

    /**
     * @brief скопировать файл
     * 
     * @param file файл для копирования
     * @param uid идентификатор пользователя владельца
     * @param gid идентификатор группы владельца
     * @return std::unique_ptr<domain::IFileObject> копия файла
     */
    virtual std::unique_ptr<domain::IFileObject> copyFile(domain::IFileObject* file,
                            domain::userId_t uid, domain::groupId_t gid) = 0;

    /**
     * @brief переместить файл в другую директорию
     * 
     * @param file файл для перемещения
     * @param target_dir путь к целевой директории
     */
    virtual void moveFile(domain::IFileObject* file, const std::string& target_dir) = 0;

    /**
     * @brief получить размер файла
     * 
     * @param file файл
     * @return size_t размер файла в байтах
     */
    virtual size_t getSize(domain::IFileObject* file) const = 0;
};

/**
 * @class FileService
 * 
 * @brief реализация сервиса работы с файлами
 * 
 * выполняет операции для создания, чтения, записи и управления файлами
 */
class FileService : public IFileService {
private:
    std::shared_ptr<domain::IFileObjectFactory> file_fac;
    std::shared_ptr<domain::IInMemoryFactory> inmem_fac;
    std::shared_ptr<domain::IACLFactory> acl_fac;

public:
    /**
     * @brief конструктор сервиса файлов
     * 
     * @param ff фабрика объектов файловой системы
     * @param imf фабрика in-memory файлов
     * @param af фабрика списков контроля доступа
     */
    FileService(std::shared_ptr<domain::IFileObjectFactory> ff,
                std::shared_ptr<domain::IInMemoryFactory> imf,
                std::shared_ptr<domain::IACLFactory> af);

    std::unique_ptr<domain::IFileObject> createFile(
        const std::string& dir_path,
        const std::string& file_name,
        domain::userId_t owner_uid,
        domain::groupId_t owner_gid) override;

    void openFileForRead(domain::IFileObject* file) override;
    void openFileForWrite(domain::IFileObject* file) override;
    void closeFile(domain::IFileObject* file) override;

    const std::string& readFile(domain::IFileObject* file) override;
    void writeFile(domain::IFileObject* file, const std::string& data) override;
    void appendToFile(domain::IFileObject* file, const std::string& data) override;

    domain::IInMemoryFile* getFileInfo(domain::IFileObject* file) override;

    void renameFile(domain::IFileObject* file, const std::string& new_name) override;
    std::unique_ptr<domain::IFileObject> copyFile(domain::IFileObject* file, 
                            domain::userId_t uid, domain::groupId_t gid) override;
    void moveFile(domain::IFileObject* file, const std::string& target_dir) override;

    size_t getSize(domain::IFileObject* file) const override;
};

} // namespace services
