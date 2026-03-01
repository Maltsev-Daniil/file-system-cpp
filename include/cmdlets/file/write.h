#pragma once

#include "cmdlets/icmdlet.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>

namespace services {
class IFileAppService;
}

namespace cmdlets {

/**
 * @class WriteCmdlet
 * 
 * @brief команда для записи данных в файл
 * 
 * записывает данные в указанный файл
 */
class WriteCmdlet : public ICmdlet {
    services::IFileAppService& app_files;
    services::IPathPatternResolver& path_resolver;

public:
    /**
     * @brief конструктор команды write
     * 
     * @param app_files сервис для работы с файлами
     * @param path_resolver резолвер путей
     */
    WriteCmdlet(
        services::IFileAppService& app_files,
        services::IPathPatternResolver& path_resolver
    )
        : app_files(app_files),
          path_resolver(path_resolver) {}

    /**
     * @brief выполнить команду write
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

}
