#include "cmdlets/rights/geteff.h"
#include "services/application/app_rights_service.h"
#include "../../../include/dto/rights/e_rights_dto.h"
#include <stdexcept>

#include "result_cmdlets/rights/e_rights_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> GeteffCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.empty() || req.args.size() > 2) {
        throw std::invalid_argument(
            "geteff: expected FILE [username]"
        );
    }

    const std::string& input_path = req.args[0];
    const std::string target_name =
        (req.args.size() == 2) ? req.args[1] : std::string{};

    std::string absolute_path =
        path_resolver.pathToAbsolute(input_path);

    auto paths =
        path_resolver.resolveGlob(absolute_path);

    if (paths.empty()) {
        throw std::runtime_error(
            "geteff: no files matched"
        );
    }

    std::vector<FileERightsDTO> effective;
    effective.reserve(paths.size());
    for (const auto& path : paths) {
        effective.push_back(
            app_rights.getFileERights(path, target_name));
        //
        // std::string rights_str;
        // for (auto r : dto.e_rights) {
        //     switch (r) {
        //     case domain::RightType::READ:
        //         rights_str += 'r';
        //         break;
        //     case domain::RightType::WRITE:
        //         rights_str += 'w';
        //         break;
        //     case domain::RightType::EXECUTE:
        //         rights_str += 'x';
        //         break;
        //     default:
        //         break;
        //     }
        // }
        //
        // output.write(
        //     "file: " + path + "\n" +
        //     "rights: " + rights_str + "\n"
        // );
    }
    return std::make_unique<FileERightsResult>(std::move(effective));
}

} // namespace cmdlets