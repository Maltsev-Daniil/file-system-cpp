#include "cmdlets/file_system/du.h"
#include "services/application/app_filesystem_service.h"
#include "../../../include/dto/fs/disk_usage_dto.h"
#include <stdexcept>

#include "../../../include/dto/fs/disk_usage_dto.h"
#include "result_cmdlets/disk_usage_result.h"

using namespace services;

namespace cmdlets {
std::unique_ptr<ICmdletResult> DuCmdlet::execute(const CmdletRequest &req)
{
    if (!req.args.empty()) {
        throw std::invalid_argument(
            "du: unexpected arguments"
        );
    }

    DiskUsageDTO dto =
        fs_app.diskUsage();

    return std::make_unique<DiskUsageResult>(std::move(dto));
}

} // namespace cmdlets