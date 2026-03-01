#pragma once
#include "../types.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <set>

namespace domain {

class IACLItem {
public:
    virtual ~IACLItem() = default;
    
    virtual RightType getPerm() const = 0;
    virtual void setPerm(RightType permission) = 0;
    
    virtual AccessType getType() const = 0;
    virtual void setType(AccessType access_type) = 0;
};

//TODO change calculation of rights, DENY should lead
class ACLItem : public IACLItem {
    RightType perm;
    AccessType type;

public:
    ACLItem(RightType permission, AccessType access_type) 
        : perm(permission), type(access_type) {}

    ACLItem(const IACLItem& item)
        : perm(item.getPerm()), type(item.getType()) {}

    RightType getPerm() const override { return perm; }
    void setPerm(RightType permission) override { perm = permission; }
    
    AccessType getType() const override { return type; }
    void setType(AccessType access_type) override { type = access_type; }

    bool operator<(const ACLItem& other) const {
        return std::tie(perm, type) < std::tie(other.perm, other.type);
    }
};

class IACL {
public:
    virtual ~IACL() = default;

    virtual void addUserPermission(userId_t uid, const IACLItem& item) = 0;
    virtual AclResult removeUserPermission(userId_t uid, const IACLItem& item) = 0;
    virtual const std::set<ACLItem>& getUserPermissions(userId_t uid) const = 0;

    virtual void addGroupPermission(groupId_t gid, const IACLItem& item) = 0;
    virtual AclResult removeGroupPermission(groupId_t gid, const IACLItem& item) = 0;
    virtual const std::set<ACLItem>& getGroupPermissions(groupId_t gid) const = 0;

    virtual bool hasUserPermission(userId_t uid,
                                   RightType perm,
                                   AccessType type) const = 0;

    virtual bool hasGroupPermission(groupId_t gid,
                                    RightType perm,
                                    AccessType type) const = 0;

    virtual void clearUserPermissions(userId_t uid) = 0;
    virtual void clearGroupPermissions(groupId_t gid) = 0;

    virtual const std::set<ACLItem>& getOwnerUserMask() const = 0;
    virtual const std::set<ACLItem>& getOwnerGroupMask() const = 0;
    virtual const std::set<ACLItem>& getOtherMask() const = 0;

    virtual void setOwnerUserMask(const std::set<ACLItem>&) = 0;
    virtual void setOwnerGroupMask(const std::set<ACLItem>&) = 0;
    virtual void setOtherMask(const std::set<ACLItem>&) = 0;

    virtual std::vector<std::pair<userId_t, std::set<ACLItem>>> getSpecUserPerms() const = 0;
    virtual std::vector<std::pair<groupId_t, std::set<ACLItem>>> getSpecGroupPerms() const = 0;

    virtual std::unique_ptr<IACL> clone() const = 0;
};

/*
Если хранить ACLItem через указатели, сравнение в set или
unordered_set будет по адресам, а не по содержимому, из-за
чего одинаковые права считались бы разными. Появляется
необходимость управлять памятью, писать хеши и компараторы
для shared_ptr, усложняется код и падает производительность.
Поэтому проще и безопаснее хранить объекты по значению.
*/

class ACL : public IACL {
private:
    std::unordered_map<userId_t, std::set<ACLItem>> specific_user_rights;
    std::unordered_map<groupId_t, std::set<ACLItem>> specific_group_rights;
    std::set<ACLItem> owner_user_rights_mask;
    std::set<ACLItem> owner_group_rights_mask;
    std::set<ACLItem> other_rights_mask;

public:
    ACL(const std::set<ACLItem>& own_user_def, const std::set<ACLItem>& own_group_def,
        const std::set<ACLItem>& other_def) :
        owner_user_rights_mask(own_user_def),
        owner_group_rights_mask(own_group_def),
        other_rights_mask(other_def) {}

    void addUserPermission(userId_t uid, const IACLItem& item) override;
    AclResult removeUserPermission(userId_t uid, const IACLItem& item) override;
    const std::set<ACLItem>& getUserPermissions(userId_t uid) const override;

    void addGroupPermission(groupId_t gid, const IACLItem& item) override;
    AclResult removeGroupPermission(groupId_t gid, const IACLItem& item) override;
    const std::set<ACLItem>& getGroupPermissions(groupId_t gid) const override;

    bool hasUserPermission(userId_t uid,
                           RightType perm,
                           AccessType type) const override;

    bool hasGroupPermission(groupId_t gid,
                            RightType perm,
                            AccessType type) const override;

    void clearUserPermissions(userId_t uid) override { specific_user_rights.erase(uid); };
    void clearGroupPermissions(groupId_t gid) override { specific_group_rights.erase(gid); };

    const std::set<ACLItem>& getOwnerUserMask() const override;
    const std::set<ACLItem>& getOwnerGroupMask() const override;
    const std::set<ACLItem>& getOtherMask() const override;
    
    void setOwnerUserMask(const std::set<ACLItem>&) override;
    void setOwnerGroupMask(const std::set<ACLItem>&) override; 
    void setOtherMask(const std::set<ACLItem>&) override;

    std::vector<std::pair<userId_t, std::set<ACLItem>>> getSpecUserPerms() const override;
    std::vector<std::pair<groupId_t, std::set<ACLItem>>> getSpecGroupPerms() const override;

    std::unique_ptr<IACL> clone() const override;
};
} // namespace domain