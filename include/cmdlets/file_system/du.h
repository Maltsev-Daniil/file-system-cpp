#pragma once

#include "cmdlets/icmdlet.h"
#include "../../io.h"
#include <memory>

namespace services {
class IFileSystemAppService;
}

namespace cmdlets {

/**
 * @class DuCmdlet
 * 
 * @brief команда для вывода использования диска
 * 
 * выводит информацию об использовании дискового пространства
 */
class DuCmdlet : public ICmdlet {
    services::IFileSystemAppService& fs_app;
    IOutput& output;

public:
    /**
     * @brief конструктор команды du
     * 
     * @param fs_app сервис для работы с файловой системой
     * @param output вывод для результатов
     */
    DuCmdlet(
        services::IFileSystemAppService& fs_app,
        IOutput& output
    ) :
        fs_app(fs_app),
        output(output) {}

    /**
     * @brief выполнить команду du
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения
     */
    std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) override;
};

} // namespace cmdlets