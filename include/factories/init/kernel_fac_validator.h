#pragma once

#include <unordered_set>

#include "domain/tables/itable.h"

namespace domain {
class IFileObject;
}

namespace domain {
class IInMemoryFactory;
class IUserFactory;
class IGroupFactory;
class IKernelContainer;
class IDirectory;
class IUser;
class IGroup;
}

// TODO учитываем что сюда надо инжектить фабрики
class KernelFactoryValidator {
public:
    KernelFactoryValidator(std::shared_ptr<domain::IGroupFactory> group_factory,
        std::shared_ptr<domain::IInMemoryFactory> inmem_factory,
        std::shared_ptr<domain::IUserFactory> user_factory) :
            group_factory_(std::move(group_factory)),
            inmem_factory_(std::move(inmem_factory)),
            user_factory_(std::move(user_factory)) {}

    void validate(const domain::IKernelContainer& kernel) const;
    void syncIdGenerators(const domain::IKernelContainer& kernel) const;

private:
    std::shared_ptr<domain::IInMemoryFactory> inmem_factory_;
    std::shared_ptr<domain::IUserFactory> user_factory_;
    std::shared_ptr<domain::IGroupFactory> group_factory_;

    void validateRootDirectory(const domain::IKernelContainer& kernel) const;
    void validateUserTable(const domain::IKernelContainer& kernel) const;
    void validateGroupTable(const domain::IKernelContainer& kernel) const;

    void validateUserGroupConsistency(
        const domain::IKernelContainer& kernel
    ) const;

    void validateObjectTableConsistency(
        const domain::IKernelContainer& kernel
    ) const;

    void validateRootOwnership(
        const domain::IKernelContainer& kernel
    ) const;

    void collectMaxInodeId(
        const domain::IDirectory& dir,
        std::uint64_t& max_id
    ) const;

    void validateObjectConsistency(
        const domain::IFileObject& obj,
        const std::unordered_set<domain::userId_t>& user_ids,
        const std::unordered_set<domain::groupId_t>& group_ids) const;

    void walkValidate(
        const domain::IDirectory& dir,
        const std::unordered_set<domain::userId_t>& user_ids,
        const std::unordered_set<domain::groupId_t>& group_ids) const;
};