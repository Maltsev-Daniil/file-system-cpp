#pragma once

#include "../../io.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>

#include "cmdlets/icmdlet.h"

namespace services {
class IDirNFileAppService;
}

namespace cmdlets {

/**
 * @class StatCmdlet
 * 
 * @brief команда для получения статистики файла или директории
 * 
 * выводит информацию о файле: время создания, изменения, размер, владельца
 */
class StatCmdlet : public ICmdlet {
    services::IDirNFileAppService& app_fs;
    services::IPathPatternResolver& path_resolver;
    IOutput& output;

public:
    /**
     * @brief конструктор команды stat
     * 
     * @param app_fs сервис для работы с файлами и директориями
     * @param path_resolver резолвер путей
     * @param output вывод для результатов
     */
    StatCmdlet(
        services::IDirNFileAppService& app_fs,
        services::IPathPatternResolver& path_resolver,
        IOutput& output
    )
        : app_fs(app_fs),
          path_resolver(path_resolver),
          output(output) {}

    /**
     * @brief выполнить команду stat
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets