#pragma once
#include <memory>

#include "../id_gen.h"
#include "domain/types.h"

namespace domain {
class IUser;

/**
 * @class UidGenerator
 * 
 * @brief генератор идентификаторов пользователей
 * 
 * генерирует последовательные идентификаторы пользователей начиная с 1000
 */
class UidGenerator : public IIdGenerator {
    userId_t next_uid{1000};
public:
    /**
     * @brief сгенерировать следующий идентификатор пользователя
     * 
     * @return uint64_t новый идентификатор пользователя
     */
    uint64_t generate() override { return next_uid++; }

    /**
     * @brief установить минимальное значение следующего идентификатора пользователя
     * 
     * @param id минимальное значение идентификатора
     */
    void advanceTo(uint64_t id) override {
        if (next_uid < id) {
            next_uid = id;
        }
    }
};

/**
 * @class IUserFactory
 * 
 * @brief интерфейс для фабрики пользователей
 * 
 * предоставляет методы для создания пользователей и администратора
 */
class IUserFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики пользователей
     */
    virtual ~IUserFactory() = default;

    /**
     * @brief создать пользователя
     * 
     * @param user_name имя пользователя
     * @param gid идентификатор группы
     * @return std::unique_ptr<IUser> созданный пользователь
     */
    virtual std::unique_ptr<IUser> createUser(const std::string& user_name, groupId_t) = 0;

    /**
     * @brief создать администратора
     * 
     * @return std::unique_ptr<IUser> созданный администратор
     */
    virtual std::unique_ptr<IUser> createAdmin() = 0;

    /**
     * @brief сгенерировать идентификатор для пользователя
     * 
     * @param user пользователь для генерации идентификатора
     */
    virtual void generateId(IUser* user) = 0;

    /**
     * @brief получить генератор идентификаторов пользователей
     * 
     * @return UidGenerator& ссылка на генератор идентификаторов
     */
    virtual UidGenerator& getGen() const = 0;
};

/**
 * @class UserFactory
 * 
 * @brief реализация фабрики пользователей
 * 
 * создает пользователей и администратора с использованием генератора идентификаторов
 */
class UserFactory : public IUserFactory {
    UidGenerator& uid_gen;
public:
    /**
     * @brief конструктор фабрики пользователей
     * 
     * @param gen генератор идентификаторов пользователей
     */
    explicit UserFactory(UidGenerator& gen) : uid_gen(gen) {}

    /**
     * @brief создать пользователя
     * 
     * @param user_name имя пользователя
     * @param gid идентификатор группы
     * @return std::unique_ptr<IUser> созданный пользователь
     */
    std::unique_ptr<IUser> createUser(const std::string& user_name, groupId_t) override;

    /**
     * @brief создать администратора
     * 
     * @return std::unique_ptr<IUser> созданный администратор
     */
    std::unique_ptr<IUser> createAdmin() override;

    /**
     * @brief сгенерировать идентификатор для пользователя
     * 
     * @param user пользователь для генерации идентификатора
     */
    void generateId(IUser* user) override;

    /**
     * @brief получить генератор идентификаторов пользователей
     * 
     * @return UidGenerator& ссылка на генератор идентификаторов
     */
    UidGenerator& getGen() const override { return uid_gen; }
};
}