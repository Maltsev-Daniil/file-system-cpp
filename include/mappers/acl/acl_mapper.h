#pragma once
#include <memory>

namespace domain {
class IACLFactory;
}

class AclSnapshot;

namespace domain {
class IACL;
}

class IAclMapper {
public:
    virtual ~IAclMapper() = default;

    virtual AclSnapshot
        toSnapshot(const domain::IACL& acl) const = 0;

    virtual std::unique_ptr<domain::IACL>
        fromSnapshot(const AclSnapshot& snapshot) const = 0;
};

class AclMapper : public IAclMapper {
    domain::IACLFactory& factory_;
    public:
    AclMapper(domain::IACLFactory& factory) : factory_(factory) {}
    AclSnapshot toSnapshot(const domain::IACL& acl) const override;
    std::unique_ptr<domain::IACL>
        fromSnapshot(const AclSnapshot& snapshot) const override;
};