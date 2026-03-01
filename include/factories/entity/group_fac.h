#pragma once
#include <cstdint>
#include <memory>

#include "../id_gen.h"
#include "domain/types.h"

namespace domain {
class IGroup;

/**
 * @class GidGenerator
 * 
 * @brief генератор идентификаторов групп
 * 
 * генерирует последовательные идентификаторы групп начиная с 1000
 */
class GidGenerator : public IIdGenerator {
    groupId_t next_gid{1000};
public:
    /**
     * @brief сгенерировать следующий идентификатор группы
     * 
     * @return uint64_t новый идентификатор группы
     */
    uint64_t generate() override { return next_gid++; }

    /**
     * @brief установить минимальное значение следующего идентификатора группы
     * 
     * @param id минимальное значение идентификатора
     */
    void advanceTo(uint64_t id) override {
        if (next_gid < id) {
            next_gid = id;
        }
    }
};

/**
 * @class IGroupFactory
 * 
 * @brief интерфейс для фабрики групп
 * 
 * предоставляет методы для создания групп и группы администраторов
 */
class IGroupFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики групп
     */
    virtual ~IGroupFactory() = default;

    /**
     * @brief создать группу
     * 
     * @param group_name имя группы
     * @return std::unique_ptr<IGroup> созданная группа
     */
    virtual std::unique_ptr<IGroup> createGroup(const std::string& group_name) = 0;

    /**
     * @brief создать группу администраторов
     * 
     * @return std::unique_ptr<IGroup> созданная группа администраторов
     */
    virtual std::unique_ptr<IGroup> createAdminGroup() = 0;

    /**
     * @brief сгенерировать идентификатор для группы
     * 
     * @param group группа для генерации идентификатора
     */
    virtual void generateId(IGroup* group) = 0;

    /**
     * @brief получить генератор идентификаторов групп
     * 
     * @return GidGenerator& ссылка на генератор идентификаторов
     */
    virtual GidGenerator& getGen() const = 0;
};

/**
 * @class GroupFactory
 * 
 * @brief реализация фабрики групп
 * 
 * создает группы и группу администраторов с использованием генератора идентификаторов
 */
class GroupFactory : public IGroupFactory {
    GidGenerator& gid_gen;
public:
    /**
     * @brief конструктор фабрики групп
     * 
     * @param gen генератор идентификаторов групп
     */
    explicit GroupFactory(GidGenerator& gen) : gid_gen(gen) {}

    /**
     * @brief создать группу
     * 
     * @param group_name имя группы
     * @return std::unique_ptr<IGroup> созданная группа
     */
    std::unique_ptr<IGroup> createGroup(const std::string& group_name) override;

    /**
     * @brief создать группу администраторов
     * 
     * @return std::unique_ptr<IGroup> созданная группа администраторов
     */
    std::unique_ptr<IGroup> createAdminGroup() override;

    /**
     * @brief сгенерировать идентификатор для группы
     * 
     * @param group группа для генерации идентификатора
     */
    void generateId(IGroup* group) override;

    /**
     * @brief получить генератор идентификаторов групп
     * 
     * @return GidGenerator& ссылка на генератор идентификаторов
     */
    GidGenerator& getGen() const override { return gid_gen; }
};
}
