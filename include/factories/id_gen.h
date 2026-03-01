#pragma once

namespace domain {
/**
 * @class IIdGenerator
 * 
 * @brief интерфейс для генератора идентификаторов
 * 
 * предоставляет методы для генерации и управления последовательностью идентификаторов
 */
class IIdGenerator {
public:
    /**
     * @brief деструктор интерфейса генератора идентификаторов
     */
    virtual ~IIdGenerator() = default;

    /**
     * @brief сгенерировать следующий идентификатор
     * 
     * @return uint64_t новый идентификатор
     */
    virtual uint64_t generate() = 0;

    /**
     * @brief установить минимальное значение следующего идентификатора
     * 
     * @param id минимальное значение идентификатора
     */
    virtual void advanceTo(uint64_t id) = 0;
};
}