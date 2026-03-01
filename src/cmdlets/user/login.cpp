#include "cmdlets/user/login.h"
#include <stdexcept>

#include "result_cmdlets/empty_result.h"
#include "services/application/app_user_service.h"

std::unique_ptr<cmdlets::ICmdletResult> cmdlets::LoginCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 1)
        throw std::runtime_error("login: expected USERNAME");

    const std::string& username = req.args[0];
    app_users.userLogin(username);
    return std::make_unique<EmptyResult>();
}