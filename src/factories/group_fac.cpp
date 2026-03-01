//
// Created by nitsir on 12/24/25.
//

#include "../../include/factories/entity/group_fac.h"

#include "../../include/domain/entity/group.h"

namespace domain {
class IGroup;

std::unique_ptr<IGroup> GroupFactory::createGroup(const std::string& group_name) {
    return std::make_unique<Group>(gid_gen.generate(), group_name);
}

std::unique_ptr<IGroup> GroupFactory::createAdminGroup()
{
    return std::make_unique<Group>(0, "Admins");
}

void GroupFactory::generateId(IGroup *group)
{
    group->setGid(gid_gen.generate());
}
}
