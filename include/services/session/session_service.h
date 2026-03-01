#pragma once

#include "domain/types.h"
#include <string>

namespace services {

/**
 * @class ISessionService
 * 
 * @brief интерфейс для сервиса сессии пользователя
 * 
 * управляет входом и выходом пользователя, текущей директорией
 */
class ISessionService {
public:
    /**
     * @brief деструктор интерфейса сервиса сессии
     */
    ~ISessionService() = default;

    /**
     * @brief выполнить вход пользователя в систему
     * 
     * @param username имя пользователя
     * @param uid идентификатор пользователя
     * @param current_path текущая директория
     */
    virtual void login(const std::string username, domain::userId_t uid,
        const std::string current_path) = 0;

    /**
     * @brief выполнить выход пользователя из системы
     */
    virtual void logout() = 0;

    /**
     * @brief получить идентификатор текущего пользователя
     * 
     * @return domain::userId_t идентификатор пользователя
     */
    virtual domain::userId_t getCurrentUser() = 0;

    /**
     * @brief получить имя текущего пользователя
     * 
     * @return const std::string& имя пользователя
     */
    virtual const std::string& getCurrentUsername() = 0;

    /**
     * @brief проверить выполнен ли вход в систему
     * 
     * @return true если пользователь вошел в систему, false иначе
     */
    virtual bool isLoggedIn() = 0;

    /**
     * @brief получить текущую директорию
     * 
     * @return const std::string& путь к текущей директории
     */
    virtual const std::string& getCurrentDir() = 0;
};

// запрещаем наследование
/**
 * @class SessionService
 * 
 * @brief реализация сервиса сессии пользователя
 * 
 * хранит состояние текущего пользователя и его рабочую директорию
 */
class SessionService final : public ISessionService {
    bool logged_in = false;
    std::string current_username;
    domain::userId_t current_user_id = -1;
    std::string current_path;
    
public:
    /**
     * @brief выполнить вход пользователя в систему
     * 
     * @param username имя пользователя
     * @param uid идентификатор пользователя
     * @param current_path текущая директория
     */
    void login(const std::string username, domain::userId_t uid,
        const std::string current_path) override;

    /**
     * @brief выполнить выход пользователя из системы
     */
    void logout() override;

    /**
     * @brief получить идентификатор текущего пользователя
     * 
     * @return domain::userId_t идентификатор пользователя
     */
    domain::userId_t getCurrentUser() override;

    /**
     * @brief получить имя текущего пользователя
     * 
     * @return const std::string& имя пользователя
     */
    const std::string& getCurrentUsername() override;

    /**
     * @brief проверить выполнен ли вход в систему
     * 
     * @return true если пользователь вошел в систему, false иначе
     */
    bool isLoggedIn() override;

    /**
     * @brief получить текущую директорию
     * 
     * @return const std::string& путь к текущей директории
     */
    const std::string& getCurrentDir() override;
};

}