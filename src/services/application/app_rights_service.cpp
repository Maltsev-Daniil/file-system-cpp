#include "services/application/app_rights_service.h"
#include "../../../include/dto/rights/e_rights_dto.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "services/domain/rights_service.h"
#include "../../../include/dto/rights/file_acl_dto.h"
#include "services/session/session_service.h"
#include "services/domain/kernel_service.h"
#include "../../../include/domain/objects/acl.h"
#include <variant>

#include "domain/entity/group.h"
#include "domain/entity/user.h"
#include "domain/objects/file_object.h"
#include "domain/objects/inmem_file.h"

using namespace domain;

namespace services {
void RightsAppService::changeMod(
    const std::string &path, 
    domain::ChmodTargetKind kind, 
    const std::string& target_name,
    const std::set<domain::ACLItem> &mask)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("chmod: user is not logged in");
    }

    auto user = kernel->resolveUser(session.getCurrentUsername());

    auto obj = kernel->resolvePath(path);
    auto acl = obj->getInMemoryFile()->getAcl();

    if (!rights->hasRight(user, obj, RightType::CHANGE_RIGHTS))
        throw std::runtime_error("chmod: access denied");

    switch (kind) {
        case ChmodTargetKind::OWNER_USER:
            acl->setOwnerUserMask(mask);
            break;
        case ChmodTargetKind::OWNER_GROUP:
            acl->setOwnerGroupMask(mask);
            break;
        case ChmodTargetKind::OTHER:
            acl->setOtherMask(mask);
            break;
        case ChmodTargetKind::SPECIFIC_USER: {
            auto targ_user = kernel->resolveUser(target_name);
            userId_t user_id = targ_user->getUid();
            acl->clearUserPermissions(user_id);
            for (const auto& it : mask)
                acl->addUserPermission(user_id, it);
            break;
        }
        case ChmodTargetKind::SPECIFIC_GROUP: {
            auto targ_group = kernel->resolveGroup(target_name);
            groupId_t group_id = targ_group->getGid();
            acl->clearGroupPermissions(group_id);
            for (const auto& it : mask) {
                acl->addGroupPermission(group_id, it);
            }
            break;
        }
        default:
            throw std::invalid_argument("chmod: chmodTargetKind is not valid");
    }
}
void RightsAppService::changeOwn(const std::string &path, 
    const std::string& new_owner_username) 
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("chown: user is not logged in");
    }

    // if not found throws
    auto user = kernel->resolveUser(session.getCurrentUsername());
    
    auto obj = kernel->resolvePath(path);
    
    // возможно только root
    if (!rights->hasRight(user, obj, RightType::CHANGE_OWNER))
    throw std::runtime_error("chown: access denied");
    
    // if not found throws
    auto new_owner = kernel->resolveUser(new_owner_username);
    userId_t new_id = new_owner->getUid();
    obj->getInMemoryFile()->setUid(new_id);
}

void RightsAppService::changeGrp(const std::string &path,
    const std::string &new_owner_groupname)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("chown: user is not logged in");
    }

    // if not found throws
    auto user = kernel->resolveUser(session.getCurrentUsername());
    
    auto obj = kernel->resolvePath(path);
    
    // возможно только root
    if (!rights->hasRight(user, obj, RightType::CHANGE_OWNER))
        throw std::runtime_error("chown: access denied");
    
    // if not found throws
    auto new_owner = kernel->resolveGroup(new_owner_groupname);
    groupId_t new_id = new_owner->getGid();
    obj->getInMemoryFile()->setGid(new_id);
}
    
FileAclDTO RightsAppService::getFileAcl(const std::string &path)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("getfacl: user is not logged in");
    }

    auto obj = kernel->resolvePath(path);
    auto inmem = obj->getInMemoryFile();
    auto acl = inmem->getAcl();

    std::string owner_name = kernel->resolveUser(
        inmem->getUid()
    )->getUsername();
    std::string owner_groupname = kernel->resolveGroup(
        inmem->getGid()
    )->getGroupname();
    
    FileAclDTO dto(
        path,
        owner_name,
        owner_groupname
    );

    dto.addEntry("user",  "", acl->getOwnerUserMask());
    dto.addEntry("group", "", acl->getOwnerGroupMask());
    dto.addEntry("other", "", acl->getOtherMask());

    for (const auto& [uid, perms] : acl->getSpecUserPerms()) {
        dto.addEntry("user",
            kernel->resolveUser(uid)->getUsername(), perms);
    }

    for (const auto& [gid, perms] : acl->getSpecGroupPerms()) {
        dto.addEntry("group", 
            kernel->resolveGroup(gid)->getGroupname(), perms);
    }

    return dto;
}
FileERightsDTO RightsAppService::getFileERights(const std::string &path, 
                const std::string& target_name)
{
    if (!session.isLoggedIn()) {
        throw std::runtime_error("getfacl: user is not logged in");
    }

    auto obj = kernel->resolvePath(path);

    std::string target_username =
        target_name.empty() ? session.getCurrentUsername() 
                            : target_name;
    auto user = kernel->resolveUser(target_username);
    
    return FileERightsDTO(rights->getEffectiveRights(user, obj));
}
}