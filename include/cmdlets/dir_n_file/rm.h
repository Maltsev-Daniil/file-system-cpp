#pragma once

#include "cmdlets/icmdlet.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>

namespace services {
class IDirNFileAppService;
}

namespace cmdlets {

/**
 * @class RmCmdlet
 * 
 * @brief команда для удаления файла или директории
 * 
 * удаляет указанный файл или директорию из файловой системы
 */
class RmCmdlet : public ICmdlet {
    services::IDirNFileAppService& app_fs;
    services::IPathPatternResolver& path_resolver;

public:
    /**
     * @brief конструктор команды rm
     * 
     * @param app_fs сервис для работы с файлами и директориями
     * @param path_resolver резолвер путей
     */
    RmCmdlet(
        services::IDirNFileAppService& app_fs,
        services::IPathPatternResolver& path_resolver
    )
        : app_fs(app_fs),
          path_resolver(path_resolver) {}

    /**
     * @brief выполнить команду rm
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets