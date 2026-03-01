#pragma once
#include <string>
#include <vector>

/**
 * @class IAliasRegistry
 * 
 * @brief интерфейс для реестра алиасов команд
 * 
 * управляет регистрацией и расширением алиасов команд
 */
class IAliasRegistry {
public:
    /**
     * @brief деструктор интерфейса реестра алиасов
     */
    virtual ~IAliasRegistry() = default;

    /**
     * @brief зарегистрировать алиас
     * 
     * @param tokens токены команды с алиасом
     */
    virtual void registerAlias(
        const std::vector<std::string>& tokens
    ) = 0;

    /**
     * @brief расширить строку если она является алиасом
     * 
     * @param line строка для проверки и расширения
     * @return std::vector<std::string> расширенная команда или исходная строка
     */
    virtual std::vector<std::string> expandIfAlias(
        const std::string& line
    ) const = 0;
};
