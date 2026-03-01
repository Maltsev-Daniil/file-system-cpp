#pragma once

#include <memory>

#include "cmdlets/icmdlet.h"
#include "../../services/application/app_filesystem_persistance_service.h"

namespace cmdlets {

/**
 * @class LoadfsCmdlet
 * 
 * @brief команда для загрузки файловой системы из файла
 * 
 * загружает состояние файловой системы из указанного файла
 */
class LoadfsCmdlet : public ICmdlet {
    services::IFileSystemPersistenceAppService& app_fs;

public:
    /**
     * @brief конструктор команды loadfs
     * 
     * @param app_fs сервис для работы с персистентностью файловой системы
     */
    explicit LoadfsCmdlet(
        services::IFileSystemPersistenceAppService& app_fs
    ) : app_fs(app_fs) {}

    /**
     * @brief выполнить команду loadfs
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(
        const CmdletRequest& req
    ) override;
};

} // namespace cmdlets