#include "factories/init/kernel_fac_validator.h"

#include <functional>

#include "factories/objects/inmem_fac.h"

#include "containers/kernel_container.h"

#include "domain/tables/itable.h"
#include "domain/entity/user.h"
#include "domain/entity/group.h"
#include "domain/objects/directory.h"
#include "domain/objects/inmem_file.h"

#include <memory>

#include "domain/objects/acl.h"
#include "domain/objects/directory_structure.h"
#include "factories/entity/group_fac.h"
#include "factories/entity/user_fac.h"

using namespace domain;

void KernelFactoryValidator::validate(
    const IKernelContainer& kernel
) const
{
    validateRootDirectory(kernel);
    validateUserTable(kernel);
    validateGroupTable(kernel);
    validateUserGroupConsistency(kernel);
    validateRootOwnership(kernel);
}

void KernelFactoryValidator::syncIdGenerators(
    const domain::IKernelContainer& kernel
) const
{
    userId_t max_user_id = 0;
    groupId_t max_group_id = 0;
    inode_t max_inode_id = 0;

    for (const auto* user : kernel.getUserTable()->getItems()) {
        if (!user) {
            throw std::logic_error("User table contains null user");
        }
        max_user_id = std::max(max_user_id, user->getUid());
    }

    for (const auto* group : kernel.getGroupTable()->getItems()) {
        if (!group) {
            throw std::logic_error("Group table contains null group");
        }
        max_group_id = std::max(max_group_id, group->getGid());
    }

    const auto* root =
        dynamic_cast<const domain::IDirectory*>(
            kernel.getRootDirectory().get()
        );

    if (!root) {
        throw std::logic_error("Root object is not a directory");
    }

    collectMaxInodeId(*root, max_inode_id);

    user_factory_->getGen().advanceTo(max_user_id + 1);
    group_factory_->getGen().advanceTo(max_group_id + 1);
    inmem_factory_->getGen().advanceTo(max_inode_id + 1);
}

void KernelFactoryValidator::collectMaxInodeId(
    const domain::IDirectory& dir,
    std::uint64_t& max_id
) const
{
    const auto& meta = dir.getInMemoryFile();
    max_id = std::max(max_id, meta->getId());

    for (const auto& child : dir.getDirectoryStructure()->getObjs()) {
        if (!child) {
            throw std::logic_error("Directory contains null child");
        }

        const auto& child_meta = child->getInMemoryFile();
        max_id = std::max(max_id, child_meta->getId());

        if (const auto* subdir = dynamic_cast<const domain::IDirectory*>(child)) {
            collectMaxInodeId(*subdir, max_id);
        }
    }
}

void KernelFactoryValidator::validateObjectConsistency(const domain::IFileObject &obj,
    const std::unordered_set<domain::userId_t> &user_ids,
    const std::unordered_set<domain::groupId_t> &group_ids) const {

    const auto* file_meta = obj.getInMemoryFile();
    if (!file_meta) {
        throw std::logic_error("File has no metadata");
    }

    if (!user_ids.contains(file_meta->getUid())) {
        throw std::logic_error("File refers to non-existent user");
    }

    if (!group_ids.contains(file_meta->getGid())) {
        throw std::logic_error("File refers to non-existent group");
    }

    const auto* file_acl = file_meta->getAcl();
    if (!file_acl) {
        throw std::logic_error("File has no ACL");
    }

    for (const auto& [uid, _] :
         file_acl->getSpecUserPerms()) {
        if (!user_ids.contains(uid)) {
            throw std::logic_error("ACL refers to non-existent user");
        }
         }

    for (const auto& [gid, _] :
         file_acl->getSpecGroupPerms()) {
        if (!group_ids.contains(gid)) {
            throw std::logic_error("ACL refers to non-existent group");
        }
         }
}

void KernelFactoryValidator::walkValidate(
    const domain::IDirectory& dir,
    const std::unordered_set<domain::userId_t>& user_ids,
    const std::unordered_set<domain::groupId_t>& group_ids) const {
    const auto* meta = dir.getInMemoryFile();
    if (!meta) {
        throw std::logic_error("Directory has no metadata");
    }

    if (!user_ids.contains(meta->getUid())) {
        throw std::logic_error("Object refers to non-existent user");
    }

    if (!group_ids.contains(meta->getGid())) {
        throw std::logic_error("Object refers to non-existent group");
    }

    const auto* acl = meta->getAcl();
    if (!acl) {
        throw std::logic_error("Object has no ACL");
    }

    for (const auto& [uid, _] : acl->getSpecUserPerms()) {
        if (!user_ids.contains(uid)) {
            throw std::logic_error("ACL refers to non-existent user");
        }
    }

    for (const auto& [gid, _] : acl->getSpecGroupPerms()) {
        if (!group_ids.contains(gid)) {
            throw std::logic_error("ACL refers to non-existent group");
        }
    }

    for (const auto* obj :
         dir.getDirectoryStructure()->getObjs()) {

        if (const auto* subdir =
                dynamic_cast<const domain::IDirectory*>(obj)) {
            walkValidate(*subdir, user_ids, group_ids);
        } else {
            validateObjectConsistency(*obj, user_ids, group_ids);
        }
    }
}

void KernelFactoryValidator::validateRootDirectory(
    const IKernelContainer& kernel
) const
{
    const auto& root = kernel.getRootDirectory();
    if (!root) {
        throw std::logic_error("Kernel validation failed: root directory is null");
    }
}

void KernelFactoryValidator::validateUserTable(
    const IKernelContainer& kernel
) const
{
    const auto& users = kernel.getUserTable();
    if (!users) {
        throw std::logic_error("Kernel validation failed: user table is null");
    }

    if (users->size() == 0) {
        throw std::logic_error("Kernel validation failed: user table is empty");
    }
}

void KernelFactoryValidator::validateGroupTable(
    const IKernelContainer& kernel
) const
{
    const auto& groups = kernel.getGroupTable();
    if (!groups) {
        throw std::logic_error("Kernel validation failed: group table is null");
    }

    if (groups->size() == 0) {
        throw std::logic_error("Kernel validation failed: group table is empty");
    }
}

void KernelFactoryValidator::validateRootOwnership(
    const IKernelContainer& kernel
) const
{
    const auto& root = kernel.getRootDirectory();
    const auto* meta = root->getInMemoryFile();

    if (!meta) {
        throw std::logic_error(
            "Kernel validation failed: root directory has no metadata"
        );
    }

    const auto& users = kernel.getUserTable();
    const auto& groups = kernel.getGroupTable();

    if (!users->contains(meta->getUid())) {
        throw std::logic_error(
            "Kernel validation failed: root owner user does not exist"
        );
    }

    if (!groups->contains(meta->getGid())) {
        throw std::logic_error(
            "Kernel validation failed: root owner group does not exist"
        );
    }
}

void
KernelFactoryValidator::validateUserGroupConsistency(
    const domain::IKernelContainer& kernel
) const
{
    const auto* user_table = kernel.getUserTable().get();
    const auto* group_table = kernel.getGroupTable().get();

    if (!user_table || !group_table) {
        throw std::logic_error("Kernel tables are null");
    }

    std::unordered_set<userId_t> user_ids;
    std::unordered_set<groupId_t> group_ids;

    for (const auto& user : user_table->getItems()) {
        user_ids.insert(user->getUid());
    }

    for (const auto& group : group_table->getItems()) {
        group_ids.insert(group->getGid());
    }

    for (const auto& user : user_table->getItems()) {
        const auto main_gid = user->getGroup();

        if (!group_ids.contains(main_gid)) {
            throw std::logic_error("User has invalid main group id");
        }

        for (auto gid : user->getSecondaryGroups()) {
            if (!group_ids.contains(gid)) {
                throw std::logic_error("User has invalid secondary group id");
            }
        }
    }

    for (const auto& group : group_table->getItems()) {
        const auto gid = group->getGid();

        for (auto uid : group->getUsers()) {
            if (!user_ids.contains(uid)) {
                throw std::logic_error("Group contains invalid user id");
            }

            const auto& user =
                user_table->getById(uid);

            const bool linked =
                user->getGroup() == gid ||
                user->getSecondaryGroups().contains(gid);

            if (!linked) {
                throw std::logic_error(
                    "Group user relation is not bidirectional"
                );
            }
        }
    }
}

void KernelFactoryValidator::validateObjectTableConsistency(
    const IKernelContainer& kernel
) const
{
    std::unordered_set<userId_t> user_ids;
    std::unordered_set<groupId_t> group_ids;

    for (const auto* user : kernel.getUserTable()->getItems()) {
        user_ids.insert(user->getUid());
    }

    for (const auto* group : kernel.getGroupTable()->getItems()) {
        group_ids.insert(group->getGid());
    }

    const auto* root = kernel.getRootDirectory().get();
    if (!root) {
        throw std::logic_error("Kernel has no root directory");
    }
    const auto dir_root = dynamic_cast<const domain::IDirectory*>(root);
    if (!dir_root) {
        throw std::logic_error("Root obj is not a directory");
    }
    walkValidate(*dir_root, user_ids, group_ids);
}