#include "cmdlets/file/cat.h"
#include "../../../include/io.h"
#include "../../../include/dto/objects/file_content_dto.h"
#include "services/application/app_file_service.h"
#include "services/session//session_service.h"
#include <stdexcept>

#include "result_cmdlets/objects/file_content_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> CatCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.empty()) {
        throw std::invalid_argument(
            "cat: missing path argument"
        );
    }

    const std::string& input_path = req.args[0];

    std::string absolute_path = path_resolver.pathToAbsolute(
        input_path
    );

    std::vector<std::string> paths =
        path_resolver.resolveGlob(absolute_path);

    if (paths.empty()) {
        throw std::runtime_error(
            "cat: no files matched"
        );
    }

    std::vector<FileContentDTO> contents;
    contents.reserve(paths.size());
    for (const auto& path : paths) {
        contents.push_back(app_files.readFile(path));
    }
    return std::make_unique<FileContentResult>(std::move(contents));
}

}