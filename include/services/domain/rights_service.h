#pragma once

#include "domain/types.h"
#include <memory>
#include <set>

namespace domain {
    class IFileObject;
    class IUser;
    class IACL;
}

namespace services {
class IUserService;
class IGroupService;

/**
 * @class IRightsService
 * 
 * @brief интерфейс для сервиса работы с правами доступа
 * 
 * предоставляет операции для проверки и получения прав доступа пользователей к файлам
 */
class IRightsService {
public:
    /**
     * @brief деструктор интерфейса сервиса прав доступа
     */
    virtual ~IRightsService() = default;
    
    /**
     * @brief проверить наличие права доступа у пользователя к файлу
     * 
     * @param user пользователь
     * @param file_object объект файловой системы
     * @param right тип права доступа
     * @return true если пользователь имеет право, false иначе
     */
    virtual bool hasRight(domain::IUser* user, 
                         domain::IFileObject* file_object, 
                         domain::RightType right) = 0;
    
    /**
     * @brief получить эффективные права доступа пользователя к файлу
     * 
     * @param user пользователь
     * @param file_object объект файловой системы
     * @return std::set<domain::RightType> набор эффективных прав доступа
     */
    virtual std::set<domain::RightType> 
        getEffectiveRights(domain::IUser* user, 
            domain::IFileObject* file_object) = 0;

    /**
     * @brief проверить наличие глобального права у пользователя
     * 
     * @param user пользователь
     * @param right тип права доступа
     * @return true если пользователь имеет глобальное право, false иначе
     */
    virtual bool hasGlobalRight(domain::IUser* user, 
            domain::RightType right) = 0;
};

/**
 * @class RightsService
 * 
 * @brief реализация сервиса работы с правами доступа
 * 
 * вычисляет эффективные права доступа на основе списка контроля доступа и групп пользователя
 */
class RightsService : public IRightsService {
    IUserService& user_service;
    IGroupService& group_service;
    
public:
    /**
     * @brief конструктор сервиса прав доступа
     * 
     * @param user_service сервис пользователей
     * @param group_service сервис групп
     */
    RightsService(IUserService& user_service, IGroupService& group_service) :
        user_service(user_service), group_service(group_service) {}

    /**
     * @brief проверить наличие права доступа у пользователя к файлу
     * 
     * @param user пользователь
     * @param file_object объект файловой системы
     * @param right тип права доступа
     * @return true если пользователь имеет право, false иначе
     */
    bool hasRight(domain::IUser* user, 
                  domain::IFileObject* file_object, 
                  domain::RightType right) override;
    
    /**
     * @brief получить эффективные права доступа пользователя к файлу
     * 
     * @param user пользователь
     * @param file_object объект файловой системы
     * @return std::set<domain::RightType> набор эффективных прав доступа
     */
    std::set<domain::RightType> getEffectiveRights(domain::IUser* user, 
                                                  domain::IFileObject* file_object) override;

    /**
     * @brief проверить наличие глобального права у пользователя
     * 
     * @param user пользователь
     * @param right тип права доступа
     * @return true если пользователь имеет глобальное право, false иначе
     */
    bool hasGlobalRight(domain::IUser* user, 
            domain::RightType right) override;

private:
    /**
     * @brief получить все группы пользователя включая подгруппы
     * 
     * @param user пользователь
     * @return std::set<domain::groupId_t> набор идентификаторов групп
     */
    std::set<domain::groupId_t> getAllGroupsForUser(domain::IUser* user) const;
};

} // namespace services