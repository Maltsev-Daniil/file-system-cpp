#include "cmdlets/rights/chown.h"
#include "services/application/app_rights_service.h"
#include <stdexcept>

#include "result_cmdlets/empty_result.h"

using namespace services;

namespace cmdlets {

domain::ChmodTargetKind ChownCmdlet::parseTargetKind(
    const std::string& target,
    std::string& out_name
) const
{
    if (target.empty()) {
        throw std::invalid_argument("chown: empty target");
    }

    if (target[0] == ':') {
        out_name = target.substr(1);
        if (out_name.empty()) {
            throw std::invalid_argument(
                "chown: empty group name"
            );
        }
        return domain::ChmodTargetKind::OWNER_GROUP;
    }

    out_name = target;
    return domain::ChmodTargetKind::OWNER_USER;
}

std::unique_ptr<ICmdletResult> ChownCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 2) {
        throw std::invalid_argument(
            "chown: expected OWNER FILE"
        );
    }

    const std::string& owner_arg = req.args[0];
    const std::string& path_arg  = req.args[1];

    std::string target_name;
    domain::ChmodTargetKind kind =
        parseTargetKind(owner_arg, target_name);

    std::string absolute_path =
        path_resolver.pathToAbsolute(path_arg);

    auto paths =
        path_resolver.resolveGlob(absolute_path);

    if (paths.empty()) {
        throw std::runtime_error(
            "chown: no files matched"
        );
    }

    for (const auto& path : paths) {
        if (kind == domain::ChmodTargetKind::OWNER_USER) {
            app_rights.changeOwn(
                path,
                target_name
            );
        } else {
            app_rights.changeGrp(
                path,
                target_name
            );
        }
    }
    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets