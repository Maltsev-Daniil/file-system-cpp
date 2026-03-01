#include "cmdlets/dir/ls.h"
#include "services/application/app_dir_service.h"
#include "../../../include/dto/objects/dir_content_dto.h"
#include <stdexcept>
#include "../../../include/dto/fs/disk_usage_dto.h"
#include "result_cmdlets/disk_usage_result.h"
#include "result_cmdlets/objects/dir_content_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> LsCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 1) {
        throw std::invalid_argument(
            "ls: expected <path>"
        );
    }

    const std::string& input_path = req.args[0];

    std::string absolute_path =
        path_resolver.pathToAbsolute(input_path);

    std::vector<std::string> dirs =
        path_resolver.resolveGlob(absolute_path);

    if (dirs.empty()) {
        throw std::runtime_error(
            "ls: no directories matched"
        );
    }

    std::vector<DirContentDTO> contents;
    contents.reserve(dirs.size());
    for (const auto& dir_path : dirs) {
        contents.push_back(app_dirs.listDirectory(dir_path));
    }
    return std::make_unique<DirContentResult>(std::move(contents));
}

} // namespace cmdlets