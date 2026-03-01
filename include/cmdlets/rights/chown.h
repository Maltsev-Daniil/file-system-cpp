#pragma once

#include "cmdlets/icmdlet.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>
#include <string>

namespace domain {
enum class ChmodTargetKind;
}

namespace services {
class IRightsAppService;
}

namespace cmdlets {

/**
 * @class ChownCmdlet
 * 
 * @brief команда для изменения владельца файла
 * 
 * изменяет владельца или группу файла или директории
 */
class ChownCmdlet : public ICmdlet {
    services::IRightsAppService& app_rights;
    services::IPathPatternResolver& path_resolver;

    domain::ChmodTargetKind parseTargetKind(
        const std::string& target,
        std::string& out_name
    ) const;

public:
    /**
     * @brief конструктор команды chown
     * 
     * @param app_rights сервис для работы с правами доступа
     * @param path_resolver резолвер путей
     */
    ChownCmdlet(
        services::IRightsAppService& app_rights,
        services::IPathPatternResolver& path_resolver
    ) : app_rights(app_rights), path_resolver(path_resolver) {}

    /**
     * @brief выполнить команду chown
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets