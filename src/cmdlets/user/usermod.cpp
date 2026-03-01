#include "cmdlets/user/usermod.h"
#include "services/application/app_user_service.h"
#include "requests/user/req_usermod.h"
#include <stdexcept>
#include <sstream>

#include "result_cmdlets/empty_result.h"

using namespace services;

namespace cmdlets {

std::vector<std::string> UsermodCmdlet::splitGroups(const std::string& s) const
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, ',')) {
        if (!item.empty()) {
            result.push_back(item);
        }
    }
    return result;
}

UsermodRequest UsermodCmdlet::parseRequest(
    const CmdletRequest& req,
    std::string& out_username
) const
{
    if (req.args.size() != 3) {
        throw std::invalid_argument(
            "usermod: expected OPTION GROUPS USERNAME"
        );
    }

    const std::string& opt  = req.args[0];
    const std::string& arg  = req.args[1];
    const std::string& user = req.args[2];

    UsermodRequest rq;

    if (opt == "-g") {
        rq.kind = UsermodOpt::CH_MAIN_GROUP;
        rq.groups = { arg };
    }
    else if (opt == "-a") {
        rq.kind = UsermodOpt::ADD_SECONDARY_GROUPS;
        rq.groups = splitGroups(arg);
        if (rq.groups.empty()) {
            throw std::invalid_argument(
                "usermod: empty group list"
            );
        }
    }
    else {
        throw std::invalid_argument(
            "usermod: unknown option"
        );
    }

    out_username = user;
    return rq;
}

std::unique_ptr<ICmdletResult> UsermodCmdlet::execute(const CmdletRequest &req)
{
    std::string username;
    UsermodRequest rq = parseRequest(req, username);

    app_users.userModify(username, rq);
    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets