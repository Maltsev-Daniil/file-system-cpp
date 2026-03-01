#include "../../../include/cmdlets/user/logout.h"

#include "result_cmdlets/empty_result.h"
#include "services/application/app_user_service.h"

namespace cmdlets {
std::unique_ptr<ICmdletResult> LogoutCmdlet::execute(const CmdletRequest &req) {
    app_users.userLogout();
    return std::make_unique<EmptyResult>();
}
}
