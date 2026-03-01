#pragma once

enum class AclSubjectKind {
    OwnerUser,
    OwnerGroup,
    Other,
    UserSpec,
    GroupSpec
};

class AclSubjectSnapshot {
public:
    AclSubjectSnapshot(
        AclSubjectKind kind,
        std::uint64_t id
    ) : kind_(kind), id_(id) {}

    AclSubjectKind getKind() const noexcept { return kind_; };
    std::uint64_t getId() const noexcept { return id_; };

private:
    AclSubjectKind kind_;
    std::uint64_t id_;
};
