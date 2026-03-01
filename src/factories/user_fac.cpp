//
// Created by nitsir on 12/24/25.
//

#include "../../include/factories/entity/user_fac.h"
#include <memory>
#include "../../include/domain/entity/user.h"
#include <string>


namespace domain {
std::unique_ptr<IUser> UserFactory::createUser(const std::string& user_name, groupId_t group_id) {
    return std::make_unique<User>(uid_gen.generate(), user_name, group_id);
}

std::unique_ptr<IUser> UserFactory::createAdmin()
{
    return std::make_unique<User>(0, std::string("admin"), 0);
}

void UserFactory::generateId(IUser *user)
{
    user->setUid(uid_gen.generate());
}
}
