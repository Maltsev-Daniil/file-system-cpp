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
 * @class GetfaclCmdlet
 * 
 * @brief команда для получения списка контроля доступа файла
 * 
 * выводит список контроля доступа для указанного файла или директории
 */
class GetfaclCmdlet : public ICmdlet {
    services::IRightsAppService& app_rights;
    services::IPathPatternResolver& path_resolver;
    IOutput& output;

public:
    /**
     * @brief конструктор команды getfacl
     * 
     * @param app_rights сервис для работы с правами доступа
     * @param path_resolver резолвер путей
     * @param output вывод для результатов
     */
    GetfaclCmdlet(
        services::IRightsAppService& app_rights,
        services::IPathPatternResolver& path_resolver,
        IOutput& output
    ) : app_rights(app_rights), path_resolver(path_resolver), output(output) {}

    /**
     * @brief выполнить команду getfacl
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets
