#include "cmdlets/file/touch.h"
#include "services/session/session_service.h"
#include "services/application/app_file_service.h"
#include <stdexcept>
#include <stdexcept>

#include "../../../debug.h"
#include "result_cmdlets/empty_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> TouchCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 2) {
        throw std::invalid_argument(
            "touch: expected <filename> <path>"
        );
    }

    const std::string& filename = req.args[0];
    const std::string& input_path = req.args[1];

    if (filename.find('/') != std::string::npos) {
        throw std::invalid_argument(
            "touch: filename must not contain '/'"
        );
    }

    std::string absolute_path =
        path_resolver.pathToAbsolute(input_path);

    std::vector<std::string> dirs =
        path_resolver.resolveGlob(absolute_path);

    if (dirs.empty()) {
        throw std::runtime_error(
            "touch: no directories matched"
        );
    }

    for (const auto& dir_path : dirs) {
        DEBUG_LOG(dir_path);
        app_files.createFile(filename, dir_path);
    }
    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets
