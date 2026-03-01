#pragma once
#include <string>
#include <vector>

/**
 * @class IAliasHandler
 * 
 * @brief интерфейс для обработчика алиасов
 * 
 * управляет хранением и операциями с алиасами команд
 */
class IAliasHandler {
public:
    /**
     * @brief деструктор интерфейса обработчика алиасов
     */
    virtual ~IAliasHandler() = default;

    /**
     * @brief добавить новый алиас
     * 
     * @param name имя алиаса
     * @param expansion расширение алиаса
     */
    virtual void add(
        const std::string& name,
        std::vector<std::string> expansion
    ) = 0;

    /**
     * @brief обновить существующий алиас
     * 
     * @param name имя алиаса
     * @param expansion новое расширение алиаса
     */
    virtual void update(
        const std::string& name,
        std::vector<std::string> expansion
    ) = 0;

    /**
     * @brief удалить алиас
     * 
     * @param name имя алиаса для удаления
     */
    virtual void remove(
        const std::string& name
    ) = 0;

    /**
     * @brief проверить существование алиаса
     * 
     * @param name имя алиаса
     * @return true если алиас существует, false иначе
     */
    virtual bool exists(
        const std::string& name
    ) const = 0;

    /**
     * @brief получить расширение алиаса
     * 
     * @param name имя алиаса
     * @return const std::vector<std::string>& расширение алиаса
     */
    virtual const std::vector<std::string>& get(
        const std::string& name
    ) const = 0;
};
