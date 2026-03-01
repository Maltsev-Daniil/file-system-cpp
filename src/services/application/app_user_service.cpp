#include "services/domain/user_service.h"
#include "services/domain/kernel_service.h"
#include "services/domain/rights_service.h"
#include "requests/user/req_usermod.h"
#include "services/application/app_user_service.h"
#include "services/session/session_service.h"
#include "domain/types.h"
#include "domain/entity/user.h"
#include "services/domain/group_service.h"
#include "domain/entity/group.h"

namespace services {
void UserAppService::userLogin(const std::string &username)
{
    auto user = kernel->resolveUser(username);
    session.login(user->getUsername(), user->getUid(), "/");
}

void UserAppService::createUser(std::string &username, const std::string &primary_group)
{
    auto group = groups->createGroup(primary_group);
    auto user = users->createUser(username, group->getGid());

    users->addUserToTable(std::move(user));
    groups->addGroupToTable(std::move(group));
}

void UserAppService::userModify(const std::string &username, UsermodRequest rq)
{
    if (rq.groups.empty())
        throw std::runtime_error("usermod: no groups were given");

    if (!session.isLoggedIn()) {
        throw std::runtime_error("usermod: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());
    if (!rights->hasGlobalRight(user, domain::RightType::USER_MODIFY)) {
        throw std::runtime_error(
            "usermod: access denied"
        );
    }
    auto user_to_mod = kernel->resolveUser(username);
    // получается что группы уже должны существовать иначе ошибка
    switch (rq.kind)
    {
    case UsermodOpt::CH_MAIN_GROUP: {
        auto new_main_group = kernel->resolveGroup(rq.groups[0]);
        auto old_gid = user_to_mod->getGroup();
        users->changeUserGroup(user_to_mod, new_main_group->getGid());
        groups->addUserToGroup(new_main_group, user_to_mod->getUid());
        groups->removeUserFromGroup(kernel->resolveGroup(old_gid), user_to_mod->getUid());
        break;
    }
    case UsermodOpt::ADD_SECONDARY_GROUPS: {
        for (const auto& gr : rq.groups) {
            auto new_second_group = kernel->resolveGroup(gr);
            users->addToSecondaryGroup(user_to_mod, new_second_group->getGid());
            groups->addUserToGroup(new_second_group, user_to_mod->getUid());
        }
        break;
    }
    default:
        break;
    }
}

void UserAppService::userLogout() {
    if (!session.isLoggedIn()) {
        throw std::runtime_error(
            "userLogout: user is not logged in");
    }
    session.logout();
}
}
