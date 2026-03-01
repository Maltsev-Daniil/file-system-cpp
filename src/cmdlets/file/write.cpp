#include "cmdlets/file/write.h"
#include "services/application/app_file_service.h"
#include <stdexcept>
#include <algorithm>

#include "result_cmdlets/empty_result.h"

using namespace services;

namespace cmdlets {

std::unique_ptr<ICmdletResult> WriteCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 2) {
        throw std::invalid_argument(
            "write: expected [-a] <data> <path>"
        );
    }

    if (req.flags.size() >= 2) {
        throw std::invalid_argument(
            "write: unexpected flag"
        );
    }

    bool append_mode = req.flags.size() == 1 && req.flags[0] == "a";

    const std::string& data = req.args[0];
    const std::string& input_path = req.args[1];

    std::string absolute_path =
        path_resolver.pathToAbsolute(input_path);

    std::vector<std::string> paths =
        path_resolver.resolveGlob(absolute_path);

    if (paths.empty()) {
        throw std::runtime_error(
            "write: no files matched"
        );
    }

    for (const auto& path : paths) {
        if (append_mode) {
            app_files.appendFile(path, data);
        } else {
            app_files.writeFile(path, data);
        }
    }

    return std::make_unique<EmptyResult>();
}

}
