#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "alias/ialias_registry.h"
#include "alias/ialias_handler.h"

/**
 * @class AliasRegistry
 * 
 * @brief реализация реестра алиасов команд
 * 
 * управляет регистрацией и расширением алиасов с поддержкой рекурсивного расширения
 */
class AliasRegistry final : public IAliasRegistry {
public:
    /**
     * @brief конструктор реестра алиасов
     * 
     * @param alias_handler обработчик алиасов
     */
    explicit AliasRegistry(
        IAliasHandler& alias_handler
    ) : alias_handler_(alias_handler) {}

    /**
     * @brief зарегистрировать алиас
     * 
     * @param tokens токены команды с алиасом
     */
    void registerAlias(
        const std::vector<std::string>& tokens
    ) override;

    /**
     * @brief расширить строку если она является алиасом
     * 
     * @param line строка для проверки и расширения
     * @return std::vector<std::string> расширенная команда или исходная строка
     */
    std::vector<std::string> expandIfAlias(
        const std::string& line
    ) const override;

private:
    IAliasHandler& alias_handler_;

    /**
     * @brief извлечь имя алиаса из токена
     * 
     * @param token токен с алиасом
     * @return std::string имя алиаса
     */
    std::string extractAliasName(
        const std::string& token
    );

    /**
     * @brief распарсить расширение алиаса
     * 
     * @param raw сырая строка расширения
     * @return std::vector<std::string> распарсенное расширение
     */
    std::vector<std::string> parseExpansion(
        const std::string& raw
    );

    /**
     * @brief удалить кавычки из строки
     * 
     * @param s строка для обработки
     * @return std::string строка без кавычек
     */
    std::string stripQuotes(
        const std::string& s
    );

    /**
     * @brief разделить строку на команды
     * 
     * @param s строка для разделения
     * @return std::vector<std::string> вектор команд
     */
    std::vector<std::string> splitCommands(
        const std::string& s
    );

    /**
     * @brief удалить пробелы в начале и конце строки
     * 
     * @param s строка для обработки
     * @return std::string обрезанная строка
     */
    std::string trim(
        const std::string& s
    );

    /**
     * @brief рекурсивно расширить алиас
     * 
     * @param line строка для расширения
     * @param visited множество посещенных алиасов для предотвращения циклов
     * @return std::vector<std::string> расширенная команда
     */
    std::vector<std::string> expandRecursive(
        const std::string& line,
        std::unordered_set<std::string>& visited
    ) const;
};
