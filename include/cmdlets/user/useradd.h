#pragma once
#include "cmdlets/icmdlet.h"
#include "requests/user/req_useradd.h"

namespace services {
class IUserAppService;
}

namespace cmdlets {

/**
 * @class UseraddCmdlet
 * 
 * @brief команда для создания нового пользователя
 * 
 * создает нового пользователя с указанным именем и основной группой
 */
class UseraddCmdlet : public ICmdlet {
    services::IUserAppService& app_users;
    UseraddRequest parseRequest(const CmdletRequest& req) const;

public:
    /**
     * @brief конструктор команды useradd
     * 
     * @param app_users сервис для работы с пользователями
     */
    explicit UseraddCmdlet(
        services::IUserAppService& app_users
    ) : app_users(app_users) {}

    /**
     * @brief выполнить команду useradd
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};
}
