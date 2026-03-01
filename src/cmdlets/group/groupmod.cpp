//
// Created by nitsir on 1/4/26.
//

#include "cmdlets/group/groupmod.h"

#include "requests/group/req_groupmod.h"
#include "result_cmdlets/empty_result.h"

namespace cmdlets {

GroupmodRequest GroupmodCmdlet::parseRequest(
    const CmdletRequest& req,
    std::string& out_groupname
)
{
    if (req.args.size() != 3) {
        throw std::invalid_argument(
            "groupmod: expected OPTION ARG GROUP"
        );
    }

    const std::string& opt   = req.args[0];
    const std::string& arg   = req.args[1];
    const std::string& group = req.args[2];

    if (arg.empty()) {
        throw std::invalid_argument(
            "groupmod: empty argument"
        );
    }

    GroupmodRequest rq;
    rq.args = { arg };

    if (opt == "-a") {
        rq.kind = GroupmodOpt::ADD_USER;
    }
    else if (opt == "-d") {
        rq.kind = GroupmodOpt::REMOVE_USER;
    }
    else if (opt == "-A") {
        rq.kind = GroupmodOpt::ADD_GROUP;
    }
    else if (opt == "-D") {
        rq.kind = GroupmodOpt::REMOVE_GROUP;
    }
    else if (opt == "-r") {
        rq.kind = GroupmodOpt::RENAME;
    }
    else {
        throw std::invalid_argument(
            "groupmod: unknown option"
        );
    }

    out_groupname = group;
    return rq;
}

std::unique_ptr<ICmdletResult> GroupmodCmdlet::execute(
    const CmdletRequest& req
)
{
    std::string groupname;
    GroupmodRequest rq = parseRequest(req, groupname);

    app_groups.groupModify(groupname, rq);

    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets
