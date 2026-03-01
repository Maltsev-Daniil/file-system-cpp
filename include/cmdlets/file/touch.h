#pragma once

#include "cmdlets/icmdlet.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>

namespace services {
class IFileAppService;
}

namespace cmdlets {
/**
 * @class TouchCmdlet
 * 
 * @brief команда для создания файла
 * 
 * создает новый файл или обновляет время модификации существующего
 */
class TouchCmdlet : public ICmdlet {
    services::IFileAppService& app_files;
    services::IPathPatternResolver& path_resolver;
public:
    /**
     * @brief конструктор команды touch
     * 
     * @param app_files сервис для работы с файлами
     * @param path_resolver резолвер путей
     */
    TouchCmdlet(services::IFileAppService& app_files,
        services::IPathPatternResolver& path_resolver)
        : app_files(app_files),
          path_resolver(path_resolver) {}

    /**
     * @brief выполнить команду touch
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};
}