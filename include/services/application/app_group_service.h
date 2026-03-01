#pragma once

#include <memory>
#include <string>

#include "app_base.h"

struct GroupmodRequest;

namespace services {
class IGroupService;
class IKernelService;
class IRightsService;
class IUserService;

/**
 * @class IGroupAppService
 * 
 * @brief интерфейс для сервиса работы с группами на уровне приложения
 * 
 * предоставляет операции для создания и модификации групп
 */
class IGroupAppService {
public:
    /**
     * @brief деструктор интерфейса сервиса групп
     */
    ~IGroupAppService() = default;

    /**
     * @brief изменить параметры группы
     * 
     * @param groupname имя группы
     * @param req запрос на изменение группы
     */
    virtual void groupModify(const std::string& groupname, GroupmodRequest req) = 0;

    // linux - groupadd
    /**
     * @brief создать новую группу
     * 
     * @param groupname имя группы
     */
    virtual void groupCreate(const std::string& groupname) = 0;
};

/**
 * @class GroupAppService
 * 
 * @brief реализация сервиса работы с группами на уровне приложения
 * 
 * координирует работу сервисов для выполнения операций с группами
 */
class GroupAppService : public IGroupAppService, public AppServiceBase {
    std::shared_ptr<IGroupService> groups;
    std::shared_ptr<IUserService> users;
    std::shared_ptr<IKernelService> kernel;
    std::shared_ptr<IRightsService> rights;
public:
    /**
     * @brief конструктор сервиса групп
     * 
     * @param groups сервис групп
     * @param users сервис пользователей
     * @param kernel сервис ядра файловой системы
     * @param rights сервис прав доступа
     * @param session сервис сессии
     */
    GroupAppService(std::shared_ptr<IGroupService> groups,
    std::shared_ptr<IUserService> users,
    std::shared_ptr<IKernelService> kernel,
    std::shared_ptr<IRightsService> rights,
    ISessionService& session
    )
        :
    AppServiceBase(session),
    groups(std::move(groups)),
    users(std::move(users)),
    kernel(std::move(kernel)),
    rights(std::move(rights)) {}

    /**
     * @brief изменить параметры группы
     * 
     * @param groupname имя группы
     * @param req запрос на изменение группы
     */
    void groupModify(const std::string& groupname, GroupmodRequest req) override;

    // linux - groupadd
    /**
     * @brief создать новую группу
     * 
     * @param groupname имя группы
     */
    void groupCreate(const std::string& groupname) override;
};
}