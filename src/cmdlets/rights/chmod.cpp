#include "cmdlets/rights/chmod.h"
#include "domain/types.h"
#include "../../../include/domain/objects/acl.h"
#include "services/application/app_rights_service.h"
#include <stdexcept>

#include "result_cmdlets/empty_result.h"

using namespace services;
using namespace domain;

namespace cmdlets {

ChmodTargetKind ChmodCmdlet::parseTargetKind(
    const std::string& target,
    std::string& out_name
) const
{
    auto pos = target.find(':');

    if (pos == std::string::npos) {
        if (target == "u") return ChmodTargetKind::OWNER_USER;
        if (target == "g") return ChmodTargetKind::OWNER_GROUP;
        if (target == "o") return ChmodTargetKind::OTHER;

        throw std::invalid_argument("chmod: invalid target");
    }

    std::string kind = target.substr(0, pos);
    out_name = target.substr(pos + 1);

    if (out_name.empty()) {
        throw std::invalid_argument("chmod: empty acl name");
    }

    if (kind == "u") return ChmodTargetKind::SPECIFIC_USER;
    if (kind == "g") return ChmodTargetKind::SPECIFIC_GROUP;

    throw std::invalid_argument("chmod: invalid acl target");
}

std::set<ACLItem> ChmodCmdlet::parseAclMask(
    const std::string& mode
) const
{
    if (mode.empty()) {
        throw std::invalid_argument("chmod: empty mode");
    }

    AccessType access_type = domain::AccessType::ALLOW;
    size_t pos = 0;

    if (mode[0] == '+') {
        access_type = AccessType::ALLOW;
        pos = 1;
    } else if (mode[0] == '-') {
        access_type = AccessType::DENY;
        pos = 1;
    }

    // если просто ввели пустой мод + // -
    if (pos >= mode.size()) {
        throw std::invalid_argument("chmod: invalid mode");
    }

    std::set<ACLItem> mask;

    for (; pos < mode.size(); ++pos) {
        RightType perm;

        switch (mode[pos]) {
        case 'r':
            perm = RightType::READ;
            break;
        case 'w':
            perm = RightType::WRITE;
            break;
        case 'x':
            perm = RightType::EXECUTE;
            break;
        default:
            throw std::invalid_argument(
                "chmod: invalid permission character"
            );
        }

        mask.emplace(perm, access_type);
    }

    return mask;
}

std::unique_ptr<ICmdletResult> ChmodCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 3) {
        throw std::invalid_argument(
            "chmod: expected TARGET MODE FILE"
        );
    }

    const std::string& target_arg = req.args[0];
    const std::string& mode_arg   = req.args[1];
    const std::string& path_arg   = req.args[2];

    std::string target_name;
    domain::ChmodTargetKind kind =
        parseTargetKind(target_arg, target_name);

    auto acl_mask = parseAclMask(mode_arg);

    std::string absolute_path =
        path_resolver.pathToAbsolute(path_arg);

    auto paths =
        path_resolver.resolveGlob(absolute_path);

    if (paths.empty()) {
        throw std::runtime_error(
            "chmod: no files matched"
        );
    }

    for (const auto& path : paths) {
        app_rights.changeMod(
            path,
            kind,
            target_name,
            acl_mask
        );
    }
    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets