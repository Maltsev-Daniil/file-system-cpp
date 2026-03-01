#pragma once

#include "cmdlets/icmdlet.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>

namespace services {
class IDirNFileAppService;
}

namespace cmdlets {

/**
 * @class CpCmdlet
 * 
 * @brief команда для копирования файла или директории
 * 
 * копирует файл или директорию в указанное место
 */
class CpCmdlet : public ICmdlet {
    services::IDirNFileAppService& app_fs;
    services::IPathPatternResolver& path_resolver;

public:
    /**
     * @brief конструктор команды cp
     * 
     * @param app_fs сервис для работы с файлами и директориями
     * @param path_resolver резолвер путей
     */
    CpCmdlet(
        services::IDirNFileAppService& app_fs,
        services::IPathPatternResolver& path_resolver
    )
        : app_fs(app_fs),
          path_resolver(path_resolver) {}

    /**
     * @brief выполнить команду cp
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets