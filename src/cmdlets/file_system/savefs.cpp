
// Created by nitsir on 12/25/25.
//

#include "cmdlets/file_system/savefs.h"

#include <stdexcept>

#include "result_cmdlets/empty_result.h"

namespace cmdlets {

std::unique_ptr<ICmdletResult> SavefsCmdlet::execute(
    const CmdletRequest& req
)
{
    if (!req.flags.empty()) {
        throw std::invalid_argument(
            "savefs: flags are not supported"
        );
    }

    if (req.args.size() != 1) {
        throw std::invalid_argument(
            "savefs: expected PATH"
        );
    }

    const std::string& path = req.args[0];

    if (path.empty()) {
        throw std::invalid_argument(
            "savefs: empty path"
        );
    }

    app_fs.save(path);

    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets