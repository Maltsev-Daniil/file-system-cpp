#pragma once

#include "cmdlets/icmdlet.h"
#include <memory>
#include <string>

namespace services {
class IUserAppService;
}

namespace cmdlets {

/**
 * @class LogoutCmdlet
 * 
 * @brief команда для выхода пользователя из системы
 * 
 * выполняет выход текущего пользователя из системы
 */
class LogoutCmdlet : public ICmdlet {
    services::IUserAppService& app_users;

public:
    /**
     * @brief конструктор команды logout
     * 
     * @param app_users сервис для работы с пользователями
     */
    explicit LogoutCmdlet(
        services::IUserAppService& app_users
    ) : app_users(app_users) {}

    /**
     * @brief выполнить команду logout
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

}
