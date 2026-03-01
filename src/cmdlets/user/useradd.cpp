#include "cmdlets/user/useradd.h"

#include "result_cmdlets/empty_result.h"
#include "services/application/app_user_service.h"

namespace cmdlets {

UseraddRequest UseraddCmdlet::parseRequest(
    const CmdletRequest& req
) const
{
    if (!req.flags.empty()) {
        throw std::invalid_argument(
            "useradd: flags are not supported"
        );
    }

    if (req.args.size() != 2) {
        throw std::invalid_argument(
            "useradd: expected USERNAME GROUP"
        );
    }

    UseraddRequest rq;
    rq.username       = req.args[0];
    rq.primary_group  = req.args[1];

    if (rq.username.empty()) {
        throw std::invalid_argument(
            "useradd: empty username"
        );
    }

    if (rq.primary_group.empty()) {
        throw std::invalid_argument(
            "useradd: empty group name"
        );
    }

    return rq;
}

std::unique_ptr<ICmdletResult> UseraddCmdlet::execute(
    const CmdletRequest& req
)
{
    UseraddRequest rq = parseRequest(req);

    app_users.createUser(
        rq.username,
        rq.primary_group
    );

    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets