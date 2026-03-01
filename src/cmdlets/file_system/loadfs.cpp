//
// Created by nitsir on 1/4/26.
//

#include "cmdlets/file_system/loadfs.h"

#include <stdexcept>

#include "result_cmdlets/empty_result.h"

namespace cmdlets {

std::unique_ptr<ICmdletResult> LoadfsCmdlet::execute(
    const CmdletRequest& req
)
{
    if (!req.flags.empty()) {
        throw std::invalid_argument(
            "loadfs: flags are not supported"
        );
    }

    if (req.args.size() != 1) {
        throw std::invalid_argument(
            "loadfs: expected PATH"
        );
    }

    const std::string& path = req.args[0];

    if (path.empty()) {
        throw std::invalid_argument(
            "loadfs: empty path"
        );
    }

    app_fs.load(path);

    return std::make_unique<EmptyResult>();
}

} // namespace cmdlets

