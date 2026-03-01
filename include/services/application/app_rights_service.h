#pragma once

#include "domain/types.h"
#include <string>
#include <set>
#include <variant>
#include <memory>

#include "app_base.h"

namespace domain {
class ACLItem;
}

struct FileAclDTO;
struct FileERightsDTO;

namespace services {
class IRightsService;
class IKernelService;
class IUserService;
class IGroupService;

/**
 * @class IRightsAppService
 * 
 * @brief интерфейс для сервиса работы с правами доступа на уровне приложения
 * 
 * предоставляет операции для изменения прав доступа и получения информации о правах
 */
class IRightsAppService {
public:
    /**
     * @brief деструктор интерфейса сервиса прав доступа
     */
    virtual ~IRightsAppService() = default;

    /**
     * @brief изменить права доступа к файлу
     * 
     * @param path путь к файлу
     * @param kind тип цели для изменения прав
     * @param target_name имя цели
     * @param mask набор прав доступа
     */
    virtual void changeMod(const std::string& path,
    domain::ChmodTargetKind kind,
    const std::string& target_name,
    const std::set<domain::ACLItem>& mask) = 0;

    /**
     * @brief изменить владельца файла
     * 
     * @param path путь к файлу
     * @param new_owner_username имя нового владельца
     */
    virtual void changeOwn(const std::string& path,
    const std::string& new_owner_username) = 0;

    /**
     * @brief изменить группу владельца файла
     * 
     * @param path путь к файлу
     * @param new_owner_groupname имя новой группы владельца
     */
    virtual void changeGrp(const std::string& path,
    const std::string& new_owner_groupname) = 0;

    /**
     * @brief получить список контроля доступа файла
     * 
     * @param path путь к файлу
     * @return FileAclDTO данные о списке контроля доступа
     */
    virtual FileAclDTO getFileAcl(const std::string& path) = 0;

    /**
     * @brief получить эффективные права доступа к файлу
     * 
     * @param path путь к файлу
     * @param target_name имя цели для проверки прав
     * @return FileERightsDTO данные об эффективных правах доступа
     */
    virtual FileERightsDTO getFileERights(const std::string& path,
            const std::string& target_name) = 0;
};

/**
 * @class RightsAppService
 * 
 * @brief реализация сервиса работы с правами доступа на уровне приложения
 * 
 * координирует работу сервисов для выполнения операций с правами доступа
 */
class RightsAppService : public IRightsAppService, public AppServiceBase {
    std::shared_ptr<IRightsService> rights;
    std::shared_ptr<IKernelService> kernel;
public:
    /**
     * @brief конструктор сервиса прав доступа
     * 
     * @param rights сервис прав доступа
     * @param kernel сервис ядра файловой системы
     * @param session сервис сессии
     */
    RightsAppService(
        std::shared_ptr<IRightsService> rights,
        std::shared_ptr<IKernelService> kernel,
        ISessionService& session
    ) :
    AppServiceBase(session),
    rights(std::move(rights)),
    kernel(std::move(kernel)) {}

    /**
     * @brief изменить права доступа к файлу
     * 
     * @param path путь к файлу
     * @param kind тип цели для изменения прав
     * @param target_name имя цели
     * @param mask набор прав доступа
     */
    void changeMod(const std::string& path,
    domain::ChmodTargetKind kind,
    const std::string& target_name,
    const std::set<domain::ACLItem>& mask) override;

    /**
     * @brief изменить владельца файла
     * 
     * @param path путь к файлу
     * @param new_owner_username имя нового владельца
     */
    void changeOwn(const std::string& path,
    const std::string& new_owner_username) override;

    /**
     * @brief изменить группу владельца файла
     * 
     * @param path путь к файлу
     * @param new_owner_groupname имя новой группы владельца
     */
    void changeGrp(const std::string& path,
    const std::string& new_owner_groupname) override;

    /**
     * @brief получить список контроля доступа файла
     * 
     * @param path путь к файлу
     * @return FileAclDTO данные о списке контроля доступа
     */
    FileAclDTO getFileAcl(const std::string& path) override;

    /**
     * @brief получить эффективные права доступа к файлу
     * 
     * @param path путь к файлу
     * @param target_name имя цели для проверки прав
     * @return FileERightsDTO данные об эффективных правах доступа
     */
    FileERightsDTO getFileERights(const std::string& path,
             const std::string& target_name) override;
};
}