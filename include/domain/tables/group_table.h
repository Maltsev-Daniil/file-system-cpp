#pragma once
#include <unordered_map>

#include "itable.h"
#include "domain/types.h"

namespace domain {
class IGroup;

class GroupTable : public IGroupTable {
private:
    std::unordered_map<std::string, std::unique_ptr<IGroup>> groups;
    std::unordered_map<groupId_t, std::string> id_to_groupname;

public:
    bool add(std::unique_ptr<IGroup> group) override;
    bool remove(const std::string& groupname) override;
    IGroup* getByName(const std::string& groupname) const override;
    IGroup* getById(groupId_t id) const override;
    bool contains(const std::string& groupname) const override;
    bool contains(groupId_t id) const override;
    size_t size() const override;

    std::vector <const IGroup*> getItems() const override;

    std::vector<std::string> listGroupnames() const;
    void clear();
};
}
