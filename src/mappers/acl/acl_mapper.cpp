//
// Created by nitsir on 12/27/25.
//

#include "mappers/acl/acl_mapper.h"
#include "factories/objects/acl_fac.h"

#include "domain/objects/acl.h"
#include "snapshots/acl/acl_snapshot.h"
#include "snapshots/acl/acl_subject_snapshot.h"

using namespace domain;

std::unique_ptr<IACL>
AclMapper::fromSnapshot(const AclSnapshot& snapshot) const {
    std::set<ACLItem> owner_user_mask;
    std::set<ACLItem> owner_group_mask;
    std::set<ACLItem> other_mask;

    std::unordered_map<userId_t, std::set<ACLItem>> user_rights;
    std::unordered_map<groupId_t, std::set<ACLItem>> group_rights;

    for (const AclItemSnapshot& item : snapshot.getItems()) {
        ACLItem acl_item(item.getPerm(), item.getType());
        const auto& subject = item.getSubject();

        switch (subject.getKind()) {
            case AclSubjectKind::OwnerUser:
                owner_user_mask.insert(acl_item);
                break;

            case AclSubjectKind::OwnerGroup:
                owner_group_mask.insert(acl_item);
                break;

            case AclSubjectKind::Other:
                other_mask.insert(acl_item);
                break;

            case AclSubjectKind::UserSpec:
                user_rights[subject.getId()].insert(acl_item);
                break;

            case AclSubjectKind::GroupSpec:
                group_rights[subject.getId()].insert(acl_item);
                break;
        }
    }

    return factory_.createFullCustomACL(
        owner_user_mask,
        owner_group_mask,
        other_mask,
        user_rights,
        group_rights
    );
}

AclSnapshot
AclMapper::toSnapshot(const IACL& acl) const {
    std::vector<AclItemSnapshot> items;

    for (const ACLItem& item : acl.getOwnerUserMask()) {
        items.emplace_back(
            AclSubjectSnapshot(
                AclSubjectKind::OwnerUser,
                std::uint64_t(-1)
            ),
            item.getPerm(),
            item.getType()
        );
    }

    for (const ACLItem& item : acl.getOwnerGroupMask()) {
        items.emplace_back(
            AclSubjectSnapshot(
                AclSubjectKind::OwnerGroup,
                std::uint64_t(-1)
            ),
            item.getPerm(),
            item.getType()
        );
    }

    for (const ACLItem& item : acl.getOtherMask()) {
        items.emplace_back(
            AclSubjectSnapshot(
                AclSubjectKind::Other,
                std::uint64_t(-1)
            ),
            item.getPerm(),
            item.getType()
        );
    }

    for (const auto& [uid, perms] : acl.getSpecUserPerms()) {
        for (const ACLItem& item : perms) {
            items.emplace_back(
                AclSubjectSnapshot(
                    AclSubjectKind::UserSpec,
                    uid
                ),
                item.getPerm(),
                item.getType()
            );
        }
    }

    for (const auto& [gid, perms] : acl.getSpecGroupPerms()) {
        for (const ACLItem& item : perms) {
            items.emplace_back(
                AclSubjectSnapshot(
                    AclSubjectKind::GroupSpec,
                    gid
                ),
                item.getPerm(),
                item.getType()
            );
        }
    }

    return AclSnapshot(std::move(items));
}