#pragma once

#include "cmdlets/icmdlet.h"
#include <memory>
#include <string>

namespace services {
class IUserAppService;
}

namespace cmdlets {

/**
 * @class LoginCmdlet
 * 
 * @brief команда для входа пользователя в систему
 * 
 * выполняет вход указанного пользователя в систему
 */
class LoginCmdlet : public ICmdlet {
    services::IUserAppService& app_users;

public:
    /**
     * @brief конструктор команды login
     * 
     * @param app_users сервис для работы с пользователями
     */
    explicit LoginCmdlet(
        services::IUserAppService& app_users
    ) : app_users(app_users) {}

    /**
     * @brief выполнить команду login
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets