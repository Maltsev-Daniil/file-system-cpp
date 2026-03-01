#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include "requests/cmdlet/req_cmdlet.h"
#include "result_cmdlets/icmdlet_result.h"

namespace view {
class ICliView;
}

namespace cmdlets {
class ICmdlet;
}

/**
 * @class ICmdDispatcher
 * 
 * @brief интерфейс для диспетчера команд
 * 
 * управляет регистрацией и выполнением команд
 */
class ICmdDispatcher {
public:
    /**
     * @brief деструктор интерфейса диспетчера команд
     */
    virtual ~ICmdDispatcher() = default;

    /**
     * @brief выполнить команду по имени
     * 
     * @param name имя команды
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<cmdlets::ICmdletResult> результат выполнения команды
     */
    virtual std::unique_ptr<cmdlets::ICmdletResult> executeCmdlet(
        const std::string& name,
        const CmdletRequest&) = 0;

    /**
     * @brief зарегистрировать команду
     * 
     * @param name имя команды
     * @param cmdlet указатель на команду для регистрации
     */
    virtual void registerCmdlet(
        const std::string& name,
        std::unique_ptr<cmdlets::ICmdlet> cmdlet
    ) = 0;

    /**
     * @brief очистить все зарегистрированные команды
     */
    virtual void clearCmdlets() = 0;
};

/**
 * @class CmdDispatcher
 * 
 * @brief реализация диспетчера команд
 * 
 * хранит команды в словаре и выполняет их по имени
 */
class CmdDispatcher : public ICmdDispatcher {
    std::unordered_map<std::string, std::unique_ptr<cmdlets::ICmdlet>> cmdlets_map;
public:
    /**
     * @brief выполнить команду по имени
     * 
     * @param name имя команды
     * @param req запрос с параметрами команды
     * @return std::unique_ptr<cmdlets::ICmdletResult> результат выполнения команды
     */
    std::unique_ptr<cmdlets::ICmdletResult> executeCmdlet(
        const std::string& name,
        const CmdletRequest&) override;

    /**
     * @brief зарегистрировать команду
     * 
     * @param name имя команды
     * @param cmdlet указатель на команду для регистрации
     */
    void registerCmdlet(
        const std::string& name,
        std::unique_ptr<cmdlets::ICmdlet> cmdlet
    ) override;

    /**
     * @brief очистить все зарегистрированные команды
     */
    void clearCmdlets() override;
};