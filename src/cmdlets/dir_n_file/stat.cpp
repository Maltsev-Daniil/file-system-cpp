#include "cmdlets/dir_n_file/stat.h"
#include "services/application/app_both_dirnfile_service.h"
#include "../../../include/dto/objects/stat_file_dto.h"
#include <chrono>
#include <stdexcept>

#include "requests/cmdlet/req_cmdlet.h"
#include "result_cmdlets/objects/stat_file_result.h"


using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> StatCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 1) {
        throw std::invalid_argument(
            "stat: expected <path>"
        );
    }

    const std::string& input_path = req.args[0];

    std::string absolute_path =
        path_resolver.pathToAbsolute(input_path);

    std::vector<std::string> targets =
        path_resolver.resolveGlob(absolute_path);

    if (targets.empty()) {
        throw std::runtime_error(
            "stat: no files matched"
        );
    }

    std::vector<StatFileDTO> stats;
    stats.reserve(targets.size());
    for (const auto& path : targets) {
        stats.push_back(app_fs.getStat(path));
    }
    return std::make_unique<StatFileResult>(std::move(stats));
}

} // namespace cmdlets