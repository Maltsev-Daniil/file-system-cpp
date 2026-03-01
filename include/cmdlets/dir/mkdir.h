#pragma once

#include "cmdlets/icmdlet.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>

namespace services {
class IDirectoryAppService;
}

namespace cmdlets {
/**
 * @class MkdirCmdlet
 * 
 * @brief команда для создания директории
 * 
 * создает новую директорию по указанному пути
 */
class MkdirCmdlet : public ICmdlet {
    services::IDirectoryAppService& app_dirs;
    services::IPathPatternResolver& path_resolver;
public:
    /**
     * @brief конструктор команды mkdir
     * 
     * @param app_dirs сервис для работы с директориями
     * @param path_resolver резолвер путей
     */
    MkdirCmdlet(services::IDirectoryAppService& app_dirs,
        services::IPathPatternResolver& path_resolver)
        : app_dirs(app_dirs),
          path_resolver(path_resolver) {}

    /**
     * @brief выполнить команду mkdir
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};
}