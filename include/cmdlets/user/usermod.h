#pragma once

#include "cmdlets/icmdlet.h"
#include <memory>
#include <string>

// usermod -g groupname username
// usermod -a group1,group2,group3 username

namespace services {
class IUserAppService;
}

struct UsermodRequest;

namespace cmdlets {

/**
 * @class UsermodCmdlet
 * 
 * @brief команда для изменения параметров пользователя
 * 
 * изменяет основную группу или добавляет пользователя во вторичные группы
 */
class UsermodCmdlet : public ICmdlet {
    services::IUserAppService& app_users;

    UsermodRequest parseRequest(
        const CmdletRequest& req,
        std::string& out_username
    ) const;
    std::vector<std::string> splitGroups(const std::string& s) const;

public:
    /**
     * @brief конструктор команды usermod
     * 
     * @param app_users сервис для работы с пользователями
     */
    explicit UsermodCmdlet(
        services::IUserAppService& app_users
    ) : app_users(app_users) {}

    /**
     * @brief выполнить команду usermod
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets