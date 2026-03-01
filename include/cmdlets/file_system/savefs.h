#pragma once

#include <memory>

#include "cmdlets/icmdlet.h"
#include "services/application/app_filesystem_persistance_service.h"

namespace cmdlets {

/**
 * @class SavefsCmdlet
 * 
 * @brief команда для сохранения файловой системы в файл
 * 
 * сохраняет текущее состояние файловой системы в указанный файл
 */
class SavefsCmdlet : public ICmdlet {
    services::IFileSystemPersistenceAppService& app_fs;

public:
    /**
     * @brief конструктор команды savefs
     * 
     * @param app_fs сервис для работы с персистентностью файловой системы
     */
    explicit SavefsCmdlet(
        services::IFileSystemPersistenceAppService& app_fs
    ) : app_fs(app_fs) {}

    /**
     * @brief выполнить команду savefs
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(
        const CmdletRequest& req
    ) override;
};

} // namespace cmdlets