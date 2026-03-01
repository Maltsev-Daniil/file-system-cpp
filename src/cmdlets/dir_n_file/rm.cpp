#include "cmdlets/dir_n_file/rm.h"
#include "services/application/app_both_dirnfile_service.h"
#include <stdexcept>

#include "result_cmdlets/empty_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> RmCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 1) {
        throw std::invalid_argument(
            "rm: expected <path>"
        );
    }

    const std::string& input_path = req.args[0];

    std::string absolute_path =
        path_resolver.pathToAbsolute(input_path);

    std::vector<std::string> targets =
        path_resolver.resolveGlob(absolute_path);

    if (targets.empty()) {
        throw std::runtime_error(
            "rm: no files matched"
        );
    }

    for (const auto& path : targets) {
        app_fs.remove(path);
    }
    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets
