#pragma once
#include "domain/types.h"
#include <vector>
#include "../../domain/objects/acl.h"
#include <memory>

namespace domain {

/**
 * @class IACLFactory
 * 
 * @brief интерфейс для фабрики списков контроля доступа
 * 
 * предоставляет методы для создания различных типов списков контроля доступа
 */
class IACLFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики списков контроля доступа
     */
    virtual ~IACLFactory() = default;

    /**
     * @brief создать список контроля доступа для корневой директории
     * 
     * @return std::unique_ptr<IACL> список контроля доступа
     */
    virtual std::unique_ptr<IACL> createRootACL(
    ) = 0;

    /**
     * @brief создать список контроля доступа по умолчанию для файла
     * 
     * @return std::unique_ptr<IACL> список контроля доступа
     */
    virtual std::unique_ptr<IACL> createDefaultFileACL(
    ) = 0;

    /**
     * @brief создать пользовательский список контроля доступа
     * 
     * @param uid идентификатор пользователя владельца
     * @param gid идентификатор группы владельца
     * @param userPerms права доступа для пользователя владельца
     * @param groupPerms права доступа для группы владельца
     * @return std::unique_ptr<IACL> список контроля доступа
     */
    virtual std::unique_ptr<IACL> createCustomACL(
        userId_t uid,
        groupId_t gid,
        const std::vector<ACLItem>& userPerms,
        const std::vector<ACLItem>& groupPerms
    ) = 0;

    /**
     * @brief создать полный пользовательский список контроля доступа
     * 
     * @param owner_user_mask маска прав для пользователя владельца
     * @param owner_group_mask маска прав для группы владельца
     * @param other_mask маска прав для других пользователей
     * @param user_rights права доступа для конкретных пользователей
     * @param group_rights права доступа для конкретных групп
     * @return std::unique_ptr<IACL> список контроля доступа
     */
    virtual std::unique_ptr<IACL> createFullCustomACL(
    const std::set<ACLItem>& owner_user_mask,
    const std::set<ACLItem>& owner_group_mask,
    const std::set<ACLItem>& other_mask,
    const std::unordered_map<userId_t, std::set<ACLItem>>& user_rights,
    const std::unordered_map<groupId_t, std::set<ACLItem>>& group_rights) = 0;
};


// TODO фабрика хранит маски, и мы можем настраивать их
// пока просто static
/**
 * @class ACLFactory
 * 
 * @brief реализация фабрики списков контроля доступа
 * 
 * создает различные типы списков контроля доступа с предопределенными масками прав
 */
class ACLFactory : public IACLFactory {
public:
    /**
     * @brief создать список контроля доступа для корневой директории
     * 
     * @return std::unique_ptr<IACL> список контроля доступа
     */
    std::unique_ptr<IACL> createRootACL(
    ) override;

    /**
     * @brief создать список контроля доступа по умолчанию для файла
     * 
     * @return std::unique_ptr<IACL> список контроля доступа
     */
    std::unique_ptr<IACL> createDefaultFileACL(
    ) override;

    /**
     * @brief создать пользовательский список контроля доступа
     * 
     * @param uid идентификатор пользователя владельца
     * @param gid идентификатор группы владельца
     * @param userPerms права доступа для пользователя владельца
     * @param groupPerms права доступа для группы владельца
     * @return std::unique_ptr<IACL> список контроля доступа
     */
    std::unique_ptr<IACL> createCustomACL(
        userId_t uid,
        groupId_t gid,
        const std::vector<ACLItem>& userPerms,
        const std::vector<ACLItem>& groupPerms
    ) override;

    /**
     * @brief создать полный пользовательский список контроля доступа
     * 
     * @param owner_user_mask маска прав для пользователя владельца
     * @param owner_group_mask маска прав для группы владельца
     * @param other_mask маска прав для других пользователей
     * @param user_rights права доступа для конкретных пользователей
     * @param group_rights права доступа для конкретных групп
     * @return std::unique_ptr<IACL> список контроля доступа
     */
    std::unique_ptr<IACL> createFullCustomACL(
        const std::set<ACLItem>& owner_user_mask,
        const std::set<ACLItem>& owner_group_mask,
        const std::set<ACLItem>& other_mask,
        const std::unordered_map<userId_t, std::set<ACLItem>>& user_rights,
        const std::unordered_map<groupId_t, std::set<ACLItem>>& group_rights)
            override;
};

} // namespace domain