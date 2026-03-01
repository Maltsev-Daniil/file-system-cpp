#pragma once

#include <string>
#include <vector>

#include "cmd_dispatcher.h"
#include "alias/ialias_registry.h"

struct CmdletRequest;

namespace view {
// TODO добавить IAliasRegistry

/**
 * @class ICmdProcessor
 * 
 * @brief интерфейс для обработки командных строк
 * 
 * обрабатывает введенную строку и выполняет команду через диспетчер
 */
class ICmdProcessor {
public:
    /**
     * @brief деструктор интерфейса процессора команд
     */
    virtual ~ICmdProcessor() = default;

    /**
     * @brief обработать введенную строку
     * 
     * @param input_line строка с командой
     * @param view представление для отображения результата
     */
    virtual void process(const std::string& input_line, ICliView& view) = 0;
};

/**
 * @class CmdProcessor
 * 
 * @brief реализация процессора команд
 * 
 * разбирает строку на токены, парсит запрос и выполняет команду
 */
class CmdProcessor : public ICmdProcessor {
    ::ICmdDispatcher& dispatcher;
    ::IAliasRegistry& alias_registry_;
public:
    /**
     * @brief конструктор процессора команд
     * 
     * @param dispatcher диспетчер команд
     * @param alias_registry реестр алиасов
     */
    CmdProcessor(
        ::ICmdDispatcher& dispatcher,
        ::IAliasRegistry& alias_registry
    )
        : dispatcher(dispatcher),
        alias_registry_(alias_registry) {}

    /**
     * @brief обработать введенную строку
     * 
     * @param input_line строка с командой
     * @param view представление для отображения результата
     */
    void process(const std::string& input_line, ICliView& view) override;
private:
    /**
     * @brief разбить строку на токены
     * 
     * @param input_line строка для разбора
     * @return std::vector<std::string> вектор токенов
     */
    std::vector<std::string> tokenize(const std::string& input_line);

    /**
     * @brief распарсить токены в запрос команды
     * 
     * @param tokens вектор токенов
     * @return std::pair<std::string, CmdletRequest> имя команды и запрос
     */
    std::pair<
        std::string,
        CmdletRequest
    > parseTokens(const std::vector<std::string>& tokens);
};
}