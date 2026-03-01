#pragma once
#include "services/session/session_service.h"

namespace services {
/**
 * @class AppServiceBase
 * 
 * @brief базовый класс для сервисов приложения
 * 
 * предоставляет доступ к сервису сессии для всех сервисов приложения
 */
class AppServiceBase {
protected:
    ISessionService& session;

    /**
     * @brief конструктор базового сервиса приложения
     * 
     * @param session сервис сессии
     */
    AppServiceBase(ISessionService& session) :
        session(session) {}
};
}
