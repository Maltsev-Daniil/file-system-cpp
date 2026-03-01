#include "services/application/app_group_service.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "domain/types.h"
#include "services/domain/rights_service.h"
#include "services/session/session_service.h"
#include "requests/group/req_groupmod.h"
#include "services/domain/kernel_service.h"
#include "domain/entity/user.h"
#include "domain/entity/group.h"

namespace services {
void GroupAppService::groupModify(const std::string &groupname, GroupmodRequest req)
{
    if (req.args.empty())
        throw std::runtime_error("groupmod: no arguments were given");

    if (!session.isLoggedIn()) {
        throw std::runtime_error("groupmod: user is not logged in");
    }
    auto user = kernel->resolveUser(session.getCurrentUser());

    //
    if (!rights->hasGlobalRight(user, domain::RightType::USER_MODIFY)) {
        throw std::runtime_error("groupmod: only root can modify groups");
    }

    auto group = kernel->resolveGroup(groupname);
    if (!group) {
        throw std::runtime_error("groupmod: couldn't resolve group");
    }

    switch (req.kind)
    {
    case GroupmodOpt::ADD_USER: {
        for (const auto& username : req.args) {
            auto user = kernel->resolveUser(username);
            
            groups->addUserToGroup(group, user->getUid());
            
            users->addToSecondaryGroup(user, group->getGid());
        }
        break;
    }
    
    case GroupmodOpt::REMOVE_USER: {
        for (const auto& username : req.args) {
            auto user = kernel->resolveUser(username);
            
            groups->removeUserFromGroup(group, user->getUid());
            
            users->removeFromSecondaryGroup(user, group->getGid());
        }
        break;
    }
    
    case GroupmodOpt::ADD_GROUP: {
        for (const auto& subgroup_name : req.args) {
            auto subgroup = kernel->resolveGroup(subgroup_name);
            
            groups->addSubgroup(group, subgroup->getGid());
        }
        break;
    }
    
    case GroupmodOpt::REMOVE_GROUP: {
        for (const auto& subgroup_name : req.args) {
            auto subgroup = kernel->resolveGroup(subgroup_name);
            
            groups->removeSubgroup(group, subgroup->getGid());
        }
        break;
    }
    
    default:
        throw std::invalid_argument("groupmod: unknown operation");
    }
}
void GroupAppService::groupCreate(const std::string &groupname)
{
    auto group = groups->createGroup(groupname);
    groups->addGroupToTable(std::move(group));
}

}