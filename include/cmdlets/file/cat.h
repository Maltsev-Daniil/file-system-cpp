#pragma once

#include "cmdlets/icmdlet.h"
#include "services/shell/path_pattern_resolver.h"
#include "../../io.h"
#include <memory>

namespace services {
class IFileAppService;
}

namespace cmdlets {

/**
 * @class CatCmdlet
 * 
 * @brief команда для вывода содержимого файла
 * 
 * выводит содержимое указанного файла в консоль
 */
class CatCmdlet : public ICmdlet {
    services::IFileAppService& app_files;
    services::IPathPatternResolver& path_resolver;
    IOutput& output;
public:
    /**
     * @brief конструктор команды cat
     * 
     * @param app_files сервис для работы с файлами
     * @param path_resolver резолвер путей
     * @param output вывод для результатов
     */
    CatCmdlet(services::IFileAppService& app_files,
        services::IPathPatternResolver& path_resolver,
        IOutput& output)
        : app_files(app_files),
        path_resolver(path_resolver),
        output(output) {}

    /**
     * @brief выполнить команду cat
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

}