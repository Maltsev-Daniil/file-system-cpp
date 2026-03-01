#pragma once

#include "acl_subject_snapshot.h"
#include "domain/types.h"

class AclItemSnapshot {
public:
    AclItemSnapshot(
        AclSubjectSnapshot subject,
        domain::RightType perm,
        domain::AccessType type
    )
        : subject(subject),
          perm(perm),
          type(type) {}

    const AclSubjectSnapshot& getSubject() const noexcept { return subject; }
    domain::RightType getPerm() const noexcept { return perm; }
    domain::AccessType getType() const noexcept { return type; }

private:
    AclSubjectSnapshot subject;
    domain::RightType perm;
    domain::AccessType type;
};