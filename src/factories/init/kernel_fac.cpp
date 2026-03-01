#include "../../../include/factories/init/kernel_fac.h"
#include "factories/id_gen.h"
#include <memory>
#include "../../../include/domain/entity/group.h"
#include "../../../include/domain/entity/user.h"
#include "../../../include/domain/objects/directory_structure.h"
#include "../../../include/domain/objects/file_object.h"
#include "../../../include/domain/objects/inmem_file.h"
#include "containers/kernel_container.h"
#include "domain/tables/group_table.h"
#include "domain/tables/user_table.h"
#include "../../../include/factories/objects/acl_fac.h"
#include "../../../include/factories/objects/file_object_fac.h"
#include "../../../include/factories/entity/group_fac.h"
#include "../../../include/factories/objects/inmem_fac.h"
#include "../../../include/factories/entity/user_fac.h"
#include "domain/tables/itable.h"
#include "factories/init/kernel_fac_validator.h"

namespace domain {

std::unique_ptr<IUserTable> KernelFactory::createTabUserWithAdmin(std::shared_ptr<IUserFactory> user_factory) {
    auto table = std::make_unique<UserTable>();
    auto admin = user_factory->createAdmin();
    table->add(std::move(admin));
    return table;
}

std::unique_ptr<IGroupTable> KernelFactory::createTabGroupWithAdmin(std::shared_ptr<IGroupFactory> group_factory) {
    auto table = std::make_unique<GroupTable>();
    auto admin_group = group_factory->createAdminGroup();
    table->add(std::move(admin_group));
    return table;
}

std::unique_ptr<IUserTable> KernelFactory::createEmptyTabUser() {
    return std::make_unique<UserTable>();
}

std::unique_ptr<IGroupTable> KernelFactory::createEmptyTabGroup() {
    return std::make_unique<GroupTable>();
}

std::unique_ptr<IFileObject> KernelFactory::createDirRoot(std::shared_ptr<IFileObjectFactory> dir_factory, std::shared_ptr<IInMemoryFactory> inmem_factory, std::shared_ptr<IACLFactory> acl_factory, userId_t admin_id, groupId_t group_admins_id)
{
    auto acl = acl_factory->createRootACL();
    auto root_file = inmem_factory->createInMemory(admin_id, group_admins_id, std::move(acl));

    auto root_dir = dir_factory->createObject("/", "/", std::move(root_file));

    return root_dir;
}

std::shared_ptr<IKernelContainer> KernelFactory::createKernelContainerColdStart(
    std::shared_ptr<IUserFactory> user_factory,
    std::shared_ptr<IGroupFactory> group_factory, 
    std::shared_ptr<IFileObjectFactory> obj_factory,
    std::shared_ptr<IInMemoryFactory> inmem_factory, 
    std::shared_ptr<IACLFactory> acl_factory)
{
    auto users = createTabUserWithAdmin(user_factory);
    auto groups = createTabGroupWithAdmin(group_factory);
    auto root_dir = createDirRoot(obj_factory, inmem_factory, acl_factory,
                        0, 0); //TODO тут сегфолт тк еще не инициализировано

    return std::make_shared<KernelContainer>(std::move(users), std::move(groups), std::move(root_dir));
}

std::shared_ptr<IKernelContainer> KernelFactory::createKernelContainerFromSnapshot(
    std::unique_ptr<IUserTable> users,
    std::unique_ptr<IGroupTable> groups,
    std::shared_ptr<IFileObject> root_dir)
{
    auto cont = std::make_shared<KernelContainer>(
        std::move(users),
        std::move(groups),
        std::move(root_dir)
    );

    validator_.validate(*cont);
    validator_.syncIdGenerators(*cont);

    return cont;
}


} // namespace domain