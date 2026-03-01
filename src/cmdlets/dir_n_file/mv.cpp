#include "cmdlets/dir_n_file/mv.h"
#include "services/application/app_both_dirnfile_service.h"
#include <stdexcept>
#include "domain/objects/directory.h"

#include "result_cmdlets/empty_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> MvCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 2) {
        throw std::invalid_argument(
            "mv: expected <source> <destination_dir>"
        );
    }

    const std::string& src_input = req.args[0];
    const std::string& dst_input = req.args[1];

    std::string src_absolute =
        path_resolver.pathToAbsolute(src_input);

    std::string dst_absolute =
        path_resolver.pathToAbsolute(dst_input);

    // раскрываем wildcard только для source
    std::vector<std::string> sources =
        path_resolver.resolveGlob(src_absolute);

    if (sources.empty()) {
        throw std::runtime_error(
            "mv: no source files matched"
        );
    }

    for (const auto& src_path : sources) {
        app_fs.move(src_path, dst_absolute);
    }
    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets