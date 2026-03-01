#include "cmdlets/dir_n_file/cp.h"
#include "services/application/app_both_dirnfile_service.h"
#include <stdexcept>
#include "domain/objects/directory.h"

#include "result_cmdlets/empty_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> CpCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 2) {
        throw std::invalid_argument(
            "cp: expected <source> <destination>"
        );
    }

    const std::string& src_input = req.args[0];
    const std::string& dst_input = req.args[1];

    std::string src_absolute =
        path_resolver.pathToAbsolute(src_input);

    std::string dst_absolute =
        path_resolver.pathToAbsolute(dst_input);

    // раскрываем wildcard ТОЛЬКО для source
    std::vector<std::string> sources =
        path_resolver.resolveGlob(src_absolute);

    if (sources.empty()) {
        throw std::runtime_error(
            "cp: no source files matched"
        );
    }

    for (const auto& src_path : sources) {
        app_fs.copy(src_path, dst_absolute);
    }
    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets