#pragma once

#include "cmdlets/icmdlet.h"
#include "services/shell/path_pattern_resolver.h"
#include <memory>
#include <set>
#include <string>

namespace domain {
enum class ChmodTargetKind;
class ACLItem;
}

namespace services {
class IRightsAppService;
}

namespace cmdlets {

/**
 * @class ChmodCmdlet
 * 
 * @brief команда для изменения прав доступа к файлу
 * 
 * изменяет права доступа для владельца, группы или других пользователей
 */
class ChmodCmdlet : public ICmdlet {
    services::IRightsAppService& app_rights;
    services::IPathPatternResolver& path_resolver;

    domain::ChmodTargetKind parseTargetKind(
        const std::string& target,
        std::string& out_target_name
    ) const;

    std::set<domain::ACLItem> parseAclMask(
        const std::string& mode
    ) const;

public:
    /**
     * @brief конструктор команды chmod
     * 
     * @param app_rights сервис для работы с правами доступа
     * @param path_resolver резолвер путей
     */
    ChmodCmdlet(
        services::IRightsAppService& app_rights,
        services::IPathPatternResolver& path_resolver
    ) : app_rights(app_rights), path_resolver(path_resolver) {}

    /**
     * @brief выполнить команду chmod
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};
}