#pragma once

#include "cmdlets/icmdlet.h"
#include "../../io.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>

namespace services {
class IDirectoryAppService;
}

namespace cmdlets {

/**
 * @class LsCmdlet
 * 
 * @brief команда для вывода содержимого директории
 * 
 * выводит список файлов и директорий в указанной директории
 */
class LsCmdlet : public ICmdlet {
    services::IDirectoryAppService& app_dirs;
    services::IPathPatternResolver& path_resolver;
    IOutput& output;

public:
    /**
     * @brief конструктор команды ls
     * 
     * @param app_dirs сервис для работы с директориями
     * @param path_resolver резолвер путей
     * @param output вывод для результатов
     */
    LsCmdlet(
        services::IDirectoryAppService& app_dirs,
        services::IPathPatternResolver& path_resolver,
        IOutput& output
    )
        : app_dirs(app_dirs),
          path_resolver(path_resolver),
          output(output) {}

    /**
     * @brief выполнить команду ls
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets