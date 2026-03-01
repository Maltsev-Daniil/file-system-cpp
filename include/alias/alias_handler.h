#pragma once
#include <unordered_map>

#include "ialias_handler.h"

/**
 * @class AliasHandler
 * 
 * @brief реализация обработчика алиасов
 * 
 * хранит алиасы в словаре и предоставляет операции для работы с ними
 */
class AliasHandler final : public IAliasHandler {
    public:
    /**
     * @brief добавить новый алиас
     * 
     * @param name имя алиаса
     * @param commands команды для расширения
     */
    void add(const std::string &name, std::vector<std::string> commands) override;

    /**
     * @brief удалить алиас
     * 
     * @param name имя алиаса для удаления
     */
    void remove(const std::string &name) override;

    /**
     * @brief обновить существующий алиас
     * 
     * @param name имя алиаса
     * @param commands новые команды для расширения
     */
    void update(const std::string &name, std::vector<std::string> commands) override;

    /**
     * @brief проверить существование алиаса
     * 
     * @param name имя алиаса
     * @return true если алиас существует, false иначе
     */
    bool exists(const std::string &name) const override;

    /**
     * @brief получить расширение алиаса
     * 
     * @param name имя алиаса
     * @return const std::vector<std::string>& расширение алиаса
     */
    const std::vector<std::string>& get(
        const std::string& name
    ) const override;
    private:
    std::pmr::unordered_map<
        std::string,
        std::vector<std::string>> aliases;
};