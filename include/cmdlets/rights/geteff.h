#pragma once

#include "cmdlets/icmdlet.h"
#include "../../io.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>

namespace services {
class IRightsAppService;
}

namespace cmdlets {

/**
 * @class GeteffCmdlet
 * 
 * @brief команда для получения эффективных прав доступа
 * 
 * выводит эффективные права доступа пользователя к указанному файлу
 */
class GeteffCmdlet : public ICmdlet {
    services::IRightsAppService& app_rights;
    services::IPathPatternResolver& path_resolver;
    IOutput& output;

public:
    /**
     * @brief конструктор команды geteff
     * 
     * @param app_rights сервис для работы с правами доступа
     * @param path_resolver резолвер путей
     * @param output вывод для результатов
     */
    GeteffCmdlet(
        services::IRightsAppService& app_rights,
        services::IPathPatternResolver& path_resolver,
        IOutput& output
    ) : app_rights(app_rights), path_resolver(path_resolver), output(output) {}


    /**
     * @brief выполнить команду geteff
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets