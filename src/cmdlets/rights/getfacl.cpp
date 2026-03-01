#include "cmdlets/rights/getfacl.h"
#include "services/application/app_rights_service.h"
#include "../../../include/dto/rights/file_acl_dto.h"
#include "../../../include/io.h"
#include <stdexcept>

#include "result_cmdlets/rights/file_acl_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> GetfaclCmdlet::execute(const CmdletRequest &req)
{
    if (req.args.size() != 1) {
        throw std::invalid_argument(
            "getfacl: expected FILE"
        );
    }

    const std::string& input_path = req.args[0];

    std::string absolute_path =
        path_resolver.pathToAbsolute(input_path);

    auto paths =
        path_resolver.resolveGlob(absolute_path);

    if (paths.empty()) {
        throw std::runtime_error(
            "getfacl: no files matched"
        );
    }

    std::vector<FileAclDTO> acls;
    acls.reserve(paths.size());
    for (const auto& path : paths) {
        acls.push_back(app_rights.getFileAcl(path));
        //
        // output.write("# file: " + dto.file_path + "\n");
        // output.write("# owner: " + dto.owner + "\n");
        // output.write("# group: " + dto.group + "\n");
        //
        // for (const auto& entry : dto.entries) {
        //     // формат:
        //     // user::rwx
        //     // user:ivan:r-x
        //     // group::rw-
        //     // other::r--
        //     std::string line = entry.type;
        //
        //     line +=
        //         ":" +
        //         entry.name +
        //         ":" +
        //         entry.perms;
        //
        //     output.write(line + "\n");
        // }
        //
        // output.write("\n");
    }
    return std::make_unique<FileAclResult>(std::move(acls));
}

} // namespace cmdlets