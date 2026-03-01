#pragma once
#include "cmdlets/icmdlet.h"
#include "services/application/app_group_service.h"

namespace cmdlets {
/**
 * @class GroupmodCmdlet
 * 
 * @brief команда для изменения параметров группы
 * 
 * изменяет имя группы, добавляет или удаляет пользователей и подгруппы
 */
class GroupmodCmdlet : public ICmdlet {
    services::IGroupAppService& app_groups;
    GroupmodRequest parseRequest(
        const CmdletRequest& req,
        std::string& out_groupname
    );
    public:
    /**
     * @brief конструктор команды groupmod
     * 
     * @param app_groups сервис для работы с группами
     */
    explicit GroupmodCmdlet(services::IGroupAppService& app_groups) :
        app_groups(app_groups) {}

    /**
     * @brief выполнить команду groupmod
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};
}
