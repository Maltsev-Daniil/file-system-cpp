#pragma once

#include "domain/types.h"
#include "domain/tables/itable.h"
#include <set>
#include <memory>

namespace domain {
class IKernelContainer;
}

namespace domain {
class IUser;
class IUserFactory;
}

namespace services {

/**
 * @class IUserService
 * 
 * @brief интерфейс для сервиса работы с пользователями
 * 
 * предоставляет операции для создания, модификации и управления пользователями
 */
class IUserService {
public:
    /**
     * @brief деструктор интерфейса сервиса пользователей
     */
    virtual ~IUserService() = default;

    /**
     * @brief создать нового пользователя
     * 
     * @param username имя пользователя
     * @param uid идентификатор пользователя
     * @return std::unique_ptr<domain::IUser> созданный пользователь
     */
    virtual std::unique_ptr<domain::IUser> createUser(const std::string& username, domain::userId_t) = 0;
    
    /**
     * @brief изменить основную группу пользователя
     * 
     * @param user пользователь
     * @param new_group_id идентификатор новой группы
     */
    virtual void changeUserGroup(domain::IUser* user, domain::groupId_t new_group_id) = 0;

    /**
     * @brief изменить имя пользователя
     * 
     * @param user пользователь
     * @param new_username новое имя пользователя
     */
    virtual void changeUsername(domain::IUser* user, const std::string& new_username) = 0;

    /**
     * @brief изменить идентификатор пользователя
     * 
     * @param user пользователь
     * @param new_uid новый идентификатор пользователя
     */
    virtual void changeUid(domain::IUser* user, domain::userId_t new_uid) = 0;

    /**
     * @brief добавить пользователя во вторичную группу
     * 
     * @param user пользователь
     * @param group_id идентификатор группы
     */
    virtual void addToSecondaryGroup(domain::IUser* user, domain::groupId_t group_id) = 0;

    /**
     * @brief удалить пользователя из вторичной группы
     * 
     * @param user пользователь
     * @param group_id идентификатор группы
     */
    virtual void removeFromSecondaryGroup(domain::IUser* user, domain::groupId_t group_id) = 0;
    
    /**
     * @brief проверить принадлежность пользователя к группе
     * 
     * @param user пользователь
     * @param group_id идентификатор группы
     * @return true если пользователь в группе, false иначе
     */
    virtual bool isUserInGroup(domain::IUser* user, domain::groupId_t group_id) const = 0;

    // храним в секондери группы только юзера
    // а в RightsService мы уже соберем через два сервиса
    // все вложенные группы и их права у пользователя
    /**
     * @brief получить все группы пользователя включая основную
     * 
     * @param user пользователь
     * @return std::set<domain::groupId_t> набор идентификаторов групп
     */
    virtual std::set<domain::groupId_t> getAllUserGroups(domain::IUser* user) const = 0;

    // тут используем фабрику, так же надо айдишник сгенерить
    /**
     * @brief скопировать пользователя
     * 
     * @param user пользователь для копирования
     * @return std::unique_ptr<domain::IUser> копия пользователя
     */
    virtual std::unique_ptr<domain::IUser> copyUser(domain::IUser* user) const = 0;

    /**
     * @brief добавить пользователя в таблицу
     * 
     * @param user пользователь для добавления
     */
    virtual void addUserToTable(std::unique_ptr<domain::IUser>) = 0;
    
    // Поиск и фильтрация
    // virtual std::vector<domain::IUser*> filterUsers(
    //     const std::vector<domain::IUser*>& users,
    //     std::function<bool(const domain::IUser*)> predicate) const = 0;
    
    // virtual domain::IUser* findUserByUid(const std::vector<domain::IUser*>& users, domain::userId_t uid) const = 0;
    // virtual domain::IUser* findUserByUsername(const std::vector<domain::IUser*>& users, const std::string& username) const = 0;
    // virtual std::vector<domain::IUser*> findUsersInGroup(
    //     const std::vector<domain::IUser*>& users,
    //     domain::groupId_t group_id) const = 0;
    
    // // Сложные операции
    // virtual void transferUserGroup(domain::IUser* user, domain::groupId_t old_group_id, domain::groupId_t new_group_id) = 0;
    
    // // Валидация
    // virtual bool validateUser(const domain::IUser* user) const = 0;
    // virtual bool areUsersEqual(const domain::IUser* user1, const domain::IUser* user2, bool compare_uid = false) const = 0;
};

/**
 * @class UserService
 * 
 * @brief реализация сервиса работы с пользователями
 * 
 * выполняет операции для создания, модификации и управления пользователями
 */
class UserService : public IUserService {
    std::shared_ptr<domain::IUserFactory> user_factory;
    domain::IKernelContainer& kernel_container_;
public:
    /**
     * @brief конструктор сервиса пользователей
     * 
     * @param user_factory фабрика пользователей
     * @param kernel_container контейнер ядра файловой системы
     */
    UserService(std::shared_ptr<domain::IUserFactory>user_factory,
        domain::IKernelContainer& kernel_container) :
        user_factory(std::move(user_factory)),
        kernel_container_(kernel_container) {}

    std::unique_ptr<domain::IUser> createUser(const std::string& username, domain::userId_t) override;
    
    void changeUserGroup(domain::IUser* user, domain::groupId_t new_group_id) override;
    void changeUsername(domain::IUser* user, const std::string& new_username) override;
    void changeUid(domain::IUser* user, domain::userId_t new_uid) override;
    void addToSecondaryGroup(domain::IUser* user, domain::groupId_t group_id) override;
    void removeFromSecondaryGroup(domain::IUser* user, domain::groupId_t group_id) override;
    
    bool isUserInGroup(domain::IUser* user, domain::groupId_t group_id) const override;
    std::set<domain::groupId_t> getAllUserGroups(domain::IUser* user) const override;
    std::unique_ptr<domain::IUser> copyUser(domain::IUser* user) const override;
    
    void addUserToTable(std::unique_ptr<domain::IUser>) override;
    // Поиск и фильтрация
    // std::vector<domain::IUser*> filterUsers(
    //     const std::vector<domain::IUser*>& users,
    //     std::function<bool(const domain::IUser*)> predicate) const override;
    
    // domain::IUser* findUserByUid(const std::vector<domain::IUser*>& users, 
    //                              domain::userId_t uid) const override;
    // domain::IUser* findUserByUsername(const std::vector<domain::IUser*>& users, 
    //                                  const std::string& username) const override;
    // std::vector<domain::IUser*> findUsersInGroup(
    //     const std::vector<domain::IUser*>& users,
    //     domain::groupId_t group_id) const override;
    
    // // Сложные операции
    // void transferUserGroup(domain::IUser* user, 
    //                       domain::groupId_t old_group_id, 
    //                       domain::groupId_t new_group_id) override;
    
    // // Валидация
    // bool validateUser(const domain::IUser* user) const override;
    // bool areUsersEqual(const domain::IUser* user1, 
    //                   const domain::IUser* user2, 
    //                   bool compare_uid = false) const override;

    // Дополнительные методы, не входящие в интерфейс (опционально)
    // bool isUserValidForOperation(const domain::IUser* user, OperationType op) const;
    // std::vector<domain::groupId_t> getCommonGroups(domain::IUser* user1, domain::IUser* user2) const;
    
// private:
    // Приватные вспомогательные методы (если нужны)
    // bool isGroupValid(domain::groupId_t group_id) const;
    // bool isUidValid(domain::userId_t uid) const;
    // void validateUserPointer(const domain::IUser* user, const std::string& operation_name) const;
};

}