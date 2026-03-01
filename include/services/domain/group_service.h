#pragma once

#include "domain/types.h"
#include "domain/tables/itable.h"
#include <memory>
#include <set>
#include <vector>
#include <functional>

namespace domain {
class IKernelContainer;
class IGroup;
    class IGroupFactory;
}

namespace services {

/**
 * @class IGroupService
 * 
 * @brief интерфейс для сервиса работы с группами
 * 
 * предоставляет операции для создания, модификации и управления группами
 */
class IGroupService {
public:
    /**
     * @brief деструктор интерфейса сервиса групп
     */
    virtual ~IGroupService() = default;

    /**
     * @brief создать новую группу
     * 
     * @param groupname имя группы
     * @return std::unique_ptr<domain::IGroup> созданная группа
     */
    virtual std::unique_ptr<domain::IGroup> 
        createGroup(const std::string& groupname) = 0;
    
    // Базовые операции с группой
    /**
     * @brief изменить имя группы
     * 
     * @param group группа
     * @param new_name новое имя группы
     */
    virtual void changeGroupName(domain::IGroup* group, const std::string& new_name) = 0;

    /**
     * @brief изменить идентификатор группы
     * 
     * @param group группа
     * @param new_gid новый идентификатор группы
     */
    virtual void changeGroupId(domain::IGroup* group, domain::groupId_t new_gid) = 0;
    
    // Управление пользователями
    /**
     * @brief добавить пользователя в группу
     * 
     * @param group группа
     * @param user_id идентификатор пользователя
     */
    virtual void addUserToGroup(domain::IGroup* group, domain::userId_t user_id) = 0;

    /**
     * @brief удалить пользователя из группы
     * 
     * @param group группа
     * @param user_id идентификатор пользователя
     */
    virtual void removeUserFromGroup(domain::IGroup* group, domain::userId_t user_id) = 0;
    
    // Управление подгруппами
    /**
     * @brief добавить подгруппу в группу
     * 
     * @param group группа
     * @param subgroup_id идентификатор подгруппы
     */
    virtual void addSubgroup(domain::IGroup* group, domain::groupId_t subgroup_id) = 0;

    /**
     * @brief удалить подгруппу из группы
     * 
     * @param group группа
     * @param subgroup_id идентификатор подгруппы
     */
    virtual void removeSubgroup(domain::IGroup* group, domain::groupId_t subgroup_id) = 0;
    
    // Проверки
    /**
     * @brief проверить наличие пользователя в группе
     * 
     * @param group группа
     * @param user_id идентификатор пользователя
     * @return true если пользователь в группе, false иначе
     */
    virtual bool isUserInGroup(domain::IGroup* group, domain::userId_t user_id) const = 0;

    /**
     * @brief проверить является ли группа подгруппой
     * 
     * @param group группа
     * @param subgroup_id идентификатор подгруппы
     * @return true если группа является подгруппой, false иначе
     */
    virtual bool isSubgroup(domain::IGroup* group, domain::groupId_t subgroup_id) const = 0;
    
    // Получение информации
    /**
     * @brief получить всех пользователей группы
     * 
     * @param group группа
     * @return std::set<domain::userId_t> набор идентификаторов пользователей
     */
    virtual std::set<domain::userId_t> getAllUsers(domain::IGroup* group) const = 0;

    /**
     * @brief получить все подгруппы группы
     * 
     * @param group группа
     * @return std::set<domain::groupId_t> набор идентификаторов подгрупп
     */
    virtual std::set<domain::groupId_t> getAllSubgroups(domain::IGroup* group) const = 0;
    
    // Копирование
    /**
     * @brief скопировать группу
     * 
     * @param group группа для копирования
     * @return std::unique_ptr<domain::IGroup> копия группы
     */
    virtual std::unique_ptr<domain::IGroup> copyGroup(domain::IGroup* group) const = 0;

    /**
     * @brief добавить группу в таблицу
     * 
     * @param group группа для добавления
     */
    virtual void addGroupToTable(std::unique_ptr<domain::IGroup> group) = 0;
};

/**
 * @class GroupService
 * 
 * @brief реализация сервиса работы с группами
 * 
 * выполняет операции для создания, модификации и управления группами
 */
class GroupService : public IGroupService {
    std::shared_ptr<domain::IGroupFactory> group_factory;
    domain::IKernelContainer& kernel_container_;
public:
    /**
     * @brief конструктор сервиса групп
     * 
     * @param group_factory фабрика групп
     * @param kernel_container контейнер ядра файловой системы
     */
    GroupService(std::shared_ptr<domain::IGroupFactory> group_factory,
        domain::IKernelContainer& kernel_container)
        : group_factory(std::move(group_factory)),
          kernel_container_(kernel_container) {}
    
    std::unique_ptr<domain::IGroup> 
        createGroup(const std::string& groupname) override;

    void changeGroupName(domain::IGroup* group, const std::string& new_name) override;
    void changeGroupId(domain::IGroup* group, domain::groupId_t new_gid) override;
    
    void addUserToGroup(domain::IGroup* group, domain::userId_t user_id) override;
    void removeUserFromGroup(domain::IGroup* group, domain::userId_t user_id) override;
    
    void addSubgroup(domain::IGroup* group, domain::groupId_t subgroup_id) override;
    void removeSubgroup(domain::IGroup* group, domain::groupId_t subgroup_id) override;
    
    bool isUserInGroup(domain::IGroup* group, domain::userId_t user_id) const override;
    bool isSubgroup(domain::IGroup* group, domain::groupId_t subgroup_id) const override;
    
    std::set<domain::userId_t> getAllUsers(domain::IGroup* group) const override;
    std::set<domain::groupId_t> getAllSubgroups(domain::IGroup* group) const override;
    
    std::unique_ptr<domain::IGroup> copyGroup(domain::IGroup* group) const override;

    void addGroupToTable(std::unique_ptr<domain::IGroup> group) override;
};

} // namespace services