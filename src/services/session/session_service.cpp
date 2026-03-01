#include "services/session/session_service.h"
#include <stdexcept>

namespace services {

// legacy
// SessionService &SessionService::getInstance()
// {
//     static SessionService instance;
//     return instance;
// }

void SessionService::login(const std::string username, domain::userId_t uid,
    const std::string current_path)
{
    current_user_id = uid;
    current_username = username;
    logged_in = true;
    this->current_path = current_path;
}

void SessionService::logout()
{
    current_user_id = -1;
    current_username.clear();
    logged_in = false;
}

domain::userId_t SessionService::getCurrentUser()
{
    if (!logged_in)
        throw std::runtime_error("getCurrentUser: no active session");
    return current_user_id;
}

const std::string &SessionService::getCurrentUsername()
{
    return current_username;
}

bool SessionService::isLoggedIn()
{
    return logged_in;
}

const std::string &SessionService::getCurrentDir()
{
    return current_path;
}
}