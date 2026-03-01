#pragma once

#include <string>
#include <memory>

#include "app_base.h"
#include "requests/user/req_usermod.h"

namespace services {
class IRightsService;
class IKernelService;
class IUserService;
class IGroupService;

/**
 * @class IUserAppService
 * 
 * @brief интерфейс для сервиса работы с пользователями на уровне приложения
 * 
 * предоставляет операции для создания, модификации и управления сессиями пользователей
 */
class IUserAppService {
public:
    /**
     * @brief деструктор интерфейса сервиса пользователей
     */
    virtual ~IUserAppService() = default;

    /**
     * @brief создать нового пользователя
     * 
     * @param username имя пользователя
     * @param primary_group основная группа пользователя
     */
    virtual void createUser(std::string& username, 
        const std::string& primary_group) = 0;

    /**
     * @brief выполнить вход пользователя в систему
     * 
     * @param username имя пользователя
     */
    virtual void userLogin(const std::string& username) = 0;

    /**
     * @brief изменить параметры пользователя
     * 
     * @param username имя пользователя
     * @param opt запрос на изменение пользователя
     */
    virtual void userModify(const std::string& username, UsermodRequest opt) = 0;

    /**
     * @brief выполнить выход пользователя из системы
     */
    virtual void userLogout() = 0;
};

/**
 * @class UserAppService
 * 
 * @brief реализация сервиса работы с пользователями на уровне приложения
 * 
 * координирует работу сервисов для выполнения операций с пользователями
 */
class UserAppService : public IUserAppService, public AppServiceBase {
    std::shared_ptr<IRightsService> rights;
    std::shared_ptr<IKernelService> kernel;
    std::shared_ptr<IUserService> users;
    std::shared_ptr<IGroupService> groups;
public:
    /**
     * @brief конструктор сервиса пользователей
     * 
     * @param rights сервис прав доступа
     * @param kernel сервис ядра файловой системы
     * @param users сервис пользователей
     * @param groups сервис групп
     * @param session сервис сессии
     */
    UserAppService(
        std::shared_ptr<IRightsService> rights,
        std::shared_ptr<IKernelService> kernel,
        std::shared_ptr<IUserService> users,
        std::shared_ptr<IGroupService> groups,
        ISessionService& session
    ) :
    AppServiceBase(session),
    rights(std::move(rights)),
        kernel(std::move(kernel)),
        users(std::move(users)),
        groups(std::move(groups)) {}

    /**
     * @brief выполнить вход пользователя в систему
     * 
     * @param username имя пользователя
     */
    void userLogin(const std::string& username) override;

    /**
     * @brief создать нового пользователя
     * 
     * @param username имя пользователя
     * @param primary_group основная группа пользователя
     */
    void createUser(std::string& username, 
        const std::string& primary_group) override;

    /**
     * @brief изменить параметры пользователя
     * 
     * @param username имя пользователя
     * @param opt запрос на изменение пользователя
     */
    void userModify(const std::string& username, UsermodRequest opt) override;

    /**
     * @brief выполнить выход пользователя из системы
     */
    void userLogout() override;
};
}