#include "cmdlets/dir/mkdir.h"
#include "services/application/app_dir_service.h"
#include <stdexcept>

#include "result_cmdlets/empty_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> MkdirCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 2) {
        throw std::invalid_argument(
            "mkdir: expected <dirname> <path>"
        );
    }

    const std::string& dirname = req.args[0];
    const std::string& input_path = req.args[1];

    if (dirname.empty() || dirname.find('/') != std::string::npos) {
        throw std::invalid_argument(
            "mkdir: dirname must not contain '/'"
        );
    }

    std::string absolute_path =
        path_resolver.pathToAbsolute(input_path);

    std::vector<std::string> dirs =
        path_resolver.resolveGlob(absolute_path);

    if (dirs.empty()) {
        throw std::runtime_error(
            "mkdir: no directories matched"
        );
    }

    for (const auto& dir_path : dirs) {
        app_dirs.createDirectory(dirname, dir_path);
    }
    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets