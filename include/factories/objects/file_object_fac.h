#pragma once
#include <memory>
#include <string>

namespace domain {
class IInMemoryFile;
class IFileObject;

/**
 * @class IFileObjectFactory
 * 
 * @brief интерфейс для фабрики объектов файловой системы
 * 
 * предоставляет метод для создания объектов файловой системы
 */
class IFileObjectFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики объектов файловой системы
     */
    virtual ~IFileObjectFactory() = default;

    /**
     * @brief создать объект файловой системы
     * 
     * @param name имя объекта
     * @param path путь к объекту
     * @param in_memory in-memory представление файла
     * @return std::unique_ptr<IFileObject> созданный объект файловой системы
     */
    virtual std::unique_ptr<IFileObject> createObject(
        const std::string& name,
        const std::string& path,
        std::unique_ptr<IInMemoryFile> in_memory
    ) = 0;
};

/**
 * @class FileFactory
 * 
 * @brief фабрика для создания файлов
 * 
 * создает обычные файлы
 */
class FileFactory : public IFileObjectFactory {
public:
    /**
     * @brief создать файл
     * 
     * @param name имя файла
     * @param path путь к файлу
     * @param in_memory in-memory представление файла
     * @return std::unique_ptr<IFileObject> созданный файл
     */
    std::unique_ptr<IFileObject> createObject (
        const std::string& name,
        const std::string& path,
        std::unique_ptr<IInMemoryFile> in_memory) override;
};

/**
 * @class DirectoryFactory
 * 
 * @brief фабрика для создания директорий
 * 
 * создает директории
 */
class DirectoryFactory : public IFileObjectFactory {
public:
    /**
     * @brief создать директорию
     * 
     * @param name имя директории
     * @param path путь к директории
     * @param in_memory in-memory представление файла
     * @return std::unique_ptr<IFileObject> созданная директория
     */
    std::unique_ptr<IFileObject> createObject (
        const std::string& name,
        const std::string& path,
        std::unique_ptr<IInMemoryFile> in_memory) override;
};
}
