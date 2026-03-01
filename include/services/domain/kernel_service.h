#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include "domain/types.h"

namespace domain {
class IFileObject;
class IUser;
class IGroup;
class IKernelContainer;
class IInMemoryFile;
class IInMemoryFactory;
class IDirectory;
}

namespace services {

/**
 * @class IKernelService
 * 
 * @brief интерфейс для сервиса ядра файловой системы
 * 
 * предоставляет базовые операции для работы с путями, файлами, пользователями и группами
 */
class IKernelService {
    public:
    /**
     * @brief деструктор интерфейса сервиса ядра
     */
    virtual ~IKernelService() = default;
    
    /**
     * @brief разрешить путь к объекту файловой системы
     * 
     * @param path путь к объекту
     * @return domain::IFileObject* указатель на объект или nullptr если не найден
     */
    virtual domain::IFileObject* resolvePath(const std::string& path) = 0;

    /**
     * @brief разбить абсолютный путь на компоненты
     * 
     * @param absolute_path абсолютный путь
     * @return std::vector<std::string> вектор компонентов пути
     */
    virtual std::vector<std::string> splitPath(const std::string& absolute_path) const = 0;

    /**
     * @brief нормализовать компоненты пути
     * 
     * @param parts компоненты пути
     * @return std::vector<std::string> нормализованные компоненты пути
     */
    virtual std::vector<std::string> normalizePathParts(
        const std::vector<std::string>& parts
    ) const = 0;

    /**
     * @brief разрешить путь к директории
     * 
     * @param normalized_parts нормализованные компоненты пути
     * @return domain::IDirectory* указатель на директорию или nullptr если не найдена
     */
    virtual domain::IDirectory* resolveDirectory(
        const std::vector<std::string>& normalized_parts
    ) const = 0;

    /**
     * @brief проверить соответствие имени паттерну
     * 
     * @param pattern паттерн для сравнения
     * @param name имя для проверки
     * @return true если имя соответствует паттерну, false иначе
     */
    virtual bool matchName(
        const std::string& pattern,
        const std::string& name
    ) const = 0;

    /**
     * @brief расширить последний компонент пути с паттерном
     * 
     * @param base_dir базовая директория
     * @param name_pattern паттерн имени
     * @return std::vector<std::string> вектор расширенных имен
     */
    virtual std::vector<std::string> expandLastPart(
        domain::IDirectory* base_dir,
        const std::string& name_pattern
    ) const = 0;

    /**
     * @brief расширить абсолютный паттерн пути
     * 
     * @param absolute_pattern абсолютный паттерн пути
     * @return std::vector<std::string> вектор разрешенных путей
     */
    virtual std::vector<std::string> expandAbsolutePattern(
        const std::string& absolute_pattern
    ) const = 0;

    /**
     * @brief разрешить пользователя по имени
     * 
     * @param username имя пользователя
     * @return domain::IUser* указатель на пользователя или nullptr если не найден
     */
    virtual domain::IUser* resolveUser(const std::string& username) = 0;

    /**
     * @brief разрешить группу по имени
     * 
     * @param groupname имя группы
     * @return domain::IGroup* указатель на группу или nullptr если не найдена
     */
    virtual domain::IGroup* resolveGroup(const std::string& groupname) = 0;

    /**
     * @brief разрешить пользователя по идентификатору
     * 
     * @param uid идентификатор пользователя
     * @return domain::IUser* указатель на пользователя или nullptr если не найден
     */
    virtual domain::IUser* resolveUser(domain::userId_t uid) = 0;

    /**
     * @brief разрешить группу по идентификатору
     * 
     * @param gid идентификатор группы
     * @return domain::IGroup* указатель на группу или nullptr если не найдена
     */
    virtual domain::IGroup* resolveGroup(domain::groupId_t gid) = 0;

    /**
     * @brief обновить время модификации объекта
     * 
     * @param obj объект файловой системы
     */
    virtual void updateModTime(domain::IFileObject* obj) = 0;

    /**
     * @brief получить in-memory представление файла
     * 
     * @param obj объект файловой системы
     * @return domain::IInMemoryFile* указатель на in-memory файл
     */
    virtual domain::IInMemoryFile* getInMemory(domain::IFileObject* obj) = 0;

    /**
     * @brief переместить объект в другую директорию
     * 
     * @param obj объект для перемещения
     * @param old_directory старая директория
     * @param new_directory новая директория
     */
    virtual void move(domain::IFileObject* obj, domain::IFileObject* old_directory, domain::IFileObject* new_directory) = 0;

    /**
     * @brief скопировать файл
     * 
     * @param file файл для копирования
     * @param uid идентификатор пользователя владельца
     * @param gid идентификатор группы владельца
     * @return std::unique_ptr<domain::IFileObject> копия файла
     */
    virtual std::unique_ptr<domain::IFileObject> copy(
        domain::IFileObject* file,
        domain::userId_t uid,
        domain::groupId_t gid
    ) = 0;

    /**
     * @brief удалить объект из файловой системы
     * 
     * @param object объект для удаления
     * @param parent_dir родительская директория
     */
    virtual void remove(
        domain::IFileObject* object,
        domain::IFileObject* parent_dir) = 0;

    /**
     * @brief получить корневую директорию
     * 
     * @return domain::IDirectory* указатель на корневую директорию
     */
    virtual domain::IDirectory* getRootDir() = 0;

    /**
     * @brief разрешить родительскую директорию объекта
     * 
     * @param obj объект файловой системы
     * @return domain::IDirectory* указатель на родительскую директорию
     */
    virtual domain::IDirectory* resolveParentDir(
        domain::IFileObject& obj) = 0;
};

/**
 * @class KernelService
 * 
 * @brief реализация сервиса ядра файловой системы
 * 
 * выполняет базовые операции для работы с путями, файлами, пользователями и группами
 */
class KernelService : public IKernelService {
private:
    std::shared_ptr<domain::IKernelContainer> container;
    std::shared_ptr<domain::IInMemoryFactory> inmem_fac;
public:
    /**
     * @brief конструктор сервиса ядра
     * 
     * @param container контейнер ядра файловой системы
     * @param inmem_fac фабрика in-memory файлов
     */
    KernelService(std::shared_ptr<domain::IKernelContainer> container,
            std::shared_ptr<domain::IInMemoryFactory> inmem_fac) :
    container(std::move(container)),
    inmem_fac(std::move(inmem_fac)) {}
    
    domain::IFileObject* resolvePath(const std::string& path) override;
    std::vector<std::string> splitPath(const std::string& path) const override;
    std::vector<std::string> normalizePathParts(
        const std::vector<std::string>& parts
    ) const override;
    domain::IDirectory* resolveDirectory(
        const std::vector<std::string>& normalized_parts
    ) const override;
    bool matchName(
        const std::string& pattern,
        const std::string& name
    ) const override;
    std::vector<std::string> expandLastPart(
        domain::IDirectory* base_dir,
        const std::string& name_pattern
    ) const override;
    std::vector<std::string> expandAbsolutePattern(
        const std::string& absolute_pattern
    ) const override;

    domain::IUser* resolveUser(const std::string& username) override;
    domain::IGroup* resolveGroup(const std::string& groupname) override;

    domain::IUser* resolveUser(domain::userId_t uid) override;
    domain::IGroup* resolveGroup(domain::groupId_t gid) override;

    domain::IInMemoryFile* getInMemory(domain::IFileObject* obj) override;
    void move(domain::IFileObject* obj, domain::IFileObject* old_directory, domain::IFileObject* new_directory) override;
    std::unique_ptr<domain::IFileObject> copy(
        domain::IFileObject* file,
        domain::userId_t uid,
        domain::groupId_t gid
    ) override;

    void remove(
        domain::IFileObject* object,
        domain::IFileObject* parent_dir) override;

    void setIdToCopiedObj(
        domain::IFileObject* file,
        domain::userId_t uid,
        domain::groupId_t gid
    );
    domain::IDirectory* getRootDir() override;
    void updateModTime(domain::IFileObject* obj) override;
    domain::IDirectory* resolveParentDir(
        domain::IFileObject& obj) override;
};
}