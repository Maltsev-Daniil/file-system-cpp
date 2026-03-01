#pragma once
#include <memory>

#include "../id_gen.h"
#include "domain/types.h"

namespace domain {
class IACL;
class IInMemoryFile;

/**
 * @class InodeGenerator
 * 
 * @brief генератор идентификаторов inode
 * 
 * генерирует последовательные идентификаторы inode начиная с 1
 */
class InodeGenerator : public IIdGenerator {
    inode_t next_inode{1};
public:
    /**
     * @brief сгенерировать следующий идентификатор inode
     * 
     * @return uint64_t новый идентификатор inode
     */
    uint64_t generate() override { return next_inode++; }

    /**
     * @brief установить минимальное значение следующего идентификатора inode
     * 
     * @param id минимальное значение идентификатора
     */
    void advanceTo(uint64_t id) override {
        if (next_inode < id) {
            next_inode = id;
        }
    }
};

/**
 * @class IInMemoryFactory
 * 
 * @brief интерфейс для фабрики in-memory файлов
 * 
 * предоставляет методы для создания in-memory представлений файлов
 */
class IInMemoryFactory {
public:
    /**
     * @brief деструктор интерфейса фабрики in-memory файлов
     */
    virtual ~IInMemoryFactory() = default;

    /**
     * @brief создать in-memory файл
     * 
     * @param user_id идентификатор пользователя владельца
     * @param group_id идентификатор группы владельца
     * @param acl список контроля доступа
     * @return std::unique_ptr<IInMemoryFile> созданный in-memory файл
     */
    virtual std::unique_ptr<IInMemoryFile> createInMemory(userId_t user_id, groupId_t group_id,
                                                          std::unique_ptr<IACL> acl = nullptr) = 0;

    /**
     * @brief создать in-memory файл с полными данными
     * 
     * @param id идентификатор inode
     * @param user_id идентификатор пользователя владельца
     * @param group_id идентификатор группы владельца
     * @param size размер файла
     * @param acl список контроля доступа
     * @param create_time время создания
     * @param modify_time время модификации
     * @return std::unique_ptr<IInMemoryFile> созданный in-memory файл
     */
    virtual std::unique_ptr<IInMemoryFile> createFullInMemory(
        inode_t id,
        userId_t user_id,
        groupId_t group_id,
        size_t size,
        std::unique_ptr<IACL> acl,
        TimePoint create_time,
        TimePoint modify_time) = 0;

    /**
     * @brief сгенерировать идентификатор для in-memory файла
     * 
     * @param inmem in-memory файл для генерации идентификатора
     */
    virtual void generateId(IInMemoryFile* inmem) = 0;

    /**
     * @brief получить генератор идентификаторов inode
     * 
     * @return InodeGenerator& ссылка на генератор идентификаторов
     */
    virtual InodeGenerator& getGen() const = 0;
};

/**
 * @class InMemoryFactory
 * 
 * @brief реализация фабрики in-memory файлов
 * 
 * создает in-memory представления файлов с использованием генератора идентификаторов
 */
class InMemoryFactory : public IInMemoryFactory {
    InodeGenerator& inode_gen;
public:
    /**
     * @brief конструктор фабрики in-memory файлов
     * 
     * @param gen генератор идентификаторов inode
     */
    explicit InMemoryFactory(InodeGenerator& gen) : inode_gen(gen) {}

    /**
     * @brief создать in-memory файл
     * 
     * @param user_id идентификатор пользователя владельца
     * @param group_id идентификатор группы владельца
     * @param acl список контроля доступа
     * @return std::unique_ptr<IInMemoryFile> созданный in-memory файл
     */
    std::unique_ptr<IInMemoryFile> createInMemory(userId_t user_id, groupId_t group_id,
                                        std::unique_ptr<IACL> acl = nullptr) override;

    /**
     * @brief получить генератор идентификаторов inode
     * 
     * @return InodeGenerator& ссылка на генератор идентификаторов
     */
    InodeGenerator& getGen() const override { return inode_gen; }

    /**
     * @brief создать in-memory файл с полными данными
     * 
     * @param id идентификатор inode
     * @param user_id идентификатор пользователя владельца
     * @param group_id идентификатор группы владельца
     * @param size размер файла
     * @param acl список контроля доступа
     * @param create_time время создания
     * @param mod_time время модификации
     * @return std::unique_ptr<IInMemoryFile> созданный in-memory файл
     */
    std::unique_ptr<IInMemoryFile> createFullInMemory(
        inode_t id,
        userId_t user_id,
        groupId_t group_id,
        size_t size,
        std::unique_ptr<IACL> acl,
        TimePoint create_time,
        TimePoint mod_time) override;

    /**
     * @brief сгенерировать идентификатор для in-memory файла
     * 
     * @param inmem in-memory файл для генерации идентификатора
     */
    void generateId(IInMemoryFile* inmem) override;
};
}
