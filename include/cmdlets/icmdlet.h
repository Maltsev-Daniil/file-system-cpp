#pragma once

#include <memory>

#include "requests/cmdlet/req_cmdlet.h"

namespace cmdlets {
class ICmdletResult;
}

namespace cmdlets {

/**
 * @class ICmdlet
 * 
 * @brief интерфейс для выполнения команд
 * 
 * определяет метод выполнения команды с запросом и возвратом результата
 */
class ICmdlet {
public:
    /**
     * @brief деструктор интерфейса команды
     */
    virtual ~ICmdlet() = default;

    /**
     * @brief выполнить команду
     * 
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<ICmdletResult> результат выполнения команды
     */
    virtual std::unique_ptr<ICmdletResult> execute(const CmdletRequest &req) = 0;
};

}
