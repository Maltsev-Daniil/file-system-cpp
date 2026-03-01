#pragma once
#include <chrono>
#include <cstdint>

namespace domain {

/**
 * @enum LockType
 * 
 * @brief тип блокировки файла
 */
enum class LockType {
    NONE, ///< нет блокировки
    READ, ///< блокировка на чтение
    READ_WRITE, ///< блокировка на чтение и запись
    WRITE ///< блокировка на запись
};

using TimePoint = std::chrono::system_clock::time_point; ///< точка времени
using userId_t = uint64_t; ///< идентификатор пользователя
using groupId_t = uint32_t; ///< идентификатор группы
using inode_t = uint64_t; ///< идентификатор inode

/**
 * @enum AccessType
 * 
 * @brief тип доступа в списке контроля доступа
 */
enum class AccessType {
    ALLOW, ///< разрешить доступ
    DENY ///< запретить доступ
};

/**
 * @enum RightType
 * 
 * @brief тип права доступа
 */
enum class RightType {
    READ, ///< право на чтение
    WRITE, ///< право на запись
    EXECUTE, ///< право на выполнение
    CHANGE_RIGHTS, ///< право на изменение прав доступа
    CHANGE_OWNER, ///< право на изменение владельца
    USER_MODIFY ///< право на модификацию пользователя
};

/**
 * @enum AclResult
 * 
 * @brief результат операции со списком контроля доступа
 */
enum class AclResult {
    Success, ///< операция успешна
    ItemNull, ///< элемент равен nullptr
    UserNotFound, ///< пользователь не найден
    GroupNotFound, ///< группа не найдена
    PermNotFound ///< право не найдено
};

/**
 * @enum ChmodTargetKind
 * 
 * @brief тип цели для изменения прав доступа
 */
enum class ChmodTargetKind {
    OWNER_USER, ///< владелец-пользователь
    OWNER_GROUP, ///< группа владельца
    OTHER, ///< другие пользователи
    SPECIFIC_USER, ///< конкретный пользователь
    SPECIFIC_GROUP ///< конкретная группа
};

}