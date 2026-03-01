#pragma once
#include <memory>
#include "domain/tables/itable.h"

namespace domain {

class IDirectory;
class IFileObject;

/**
 * @class IKernelContainer
 * 
 * @brief интерфейс для контейнера ядра файловой системы
 * 
 * хранит таблицы пользователей, групп и корневую директорию
 */
class IKernelContainer {
public:
    /**
     * @brief деструктор интерфейса контейнера ядра
     */
    virtual ~IKernelContainer() = default;

    /**
     * @brief получить таблицу пользователей
     * 
     * @return std::shared_ptr<IUserTable> таблица пользователей
     */
    virtual std::shared_ptr<IUserTable> getUserTable() const = 0;

    /**
     * @brief получить таблицу групп
     * 
     * @return std::shared_ptr<IGroupTable> таблица групп
     */
    virtual std::shared_ptr<IGroupTable> getGroupTable() const = 0;

    /**
     * @brief получить корневую директорию
     * 
     * @return std::shared_ptr<IFileObject> корневая директория
     */
    virtual std::shared_ptr<IFileObject> getRootDirectory() const = 0;
};

/**
 * @class KernelContainer
 * 
 * @brief реализация контейнера ядра файловой системы
 * 
 * хранит таблицы пользователей, групп и корневую директорию
 */
class KernelContainer : public IKernelContainer {
private:
    std::shared_ptr<IUserTable> users;
    std::shared_ptr<IGroupTable> groups;
    std::shared_ptr<IFileObject> root_dir;
    
public:
    /**
     * @brief конструктор контейнера ядра
     * 
     * @param users таблица пользователей
     * @param groups таблица групп
     * @param root_dir корневая директория
     */
    KernelContainer(std::shared_ptr<IUserTable> users,
                    std::shared_ptr<IGroupTable> groups,
                    std::shared_ptr<IFileObject> root_dir)
        : users(std::move(users))
        , groups(std::move(groups))
        , root_dir(std::move(root_dir))
    {}

    /**
     * @brief получить таблицу пользователей
     * 
     * @return std::shared_ptr<IUserTable> таблица пользователей
     */
    std::shared_ptr<IUserTable> getUserTable() const override {
        return users;
    }

    /**
     * @brief получить таблицу групп
     * 
     * @return std::shared_ptr<IGroupTable> таблица групп
     */
    std::shared_ptr<IGroupTable> getGroupTable() const override {
        return groups;
    }

    /**
     * @brief получить корневую директорию
     * 
     * @return std::shared_ptr<IFileObject> корневая директория
     */
    std::shared_ptr<IFileObject> getRootDirectory() const override {
        return root_dir;
    }
};

} // namespace domain