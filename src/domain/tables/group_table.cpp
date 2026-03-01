#include "domain/tables/group_table.h"
#include "../../../include/domain/entity/group.h"
//
// Created by nitsir on 12/25/25.
//
namespace domain {

bool GroupTable::add(std::unique_ptr<IGroup> group) {
    if (!group) return false;

    std::string groupname = group->getGroupname();
    groupId_t gid = group->getGid();

    if (groupname.empty()) return false;

    if (groups.count(groupname) > 0 ||
        id_to_groupname.count(gid) > 0) {
        return false;
        }

    groups.emplace(groupname, std::move(group));
    id_to_groupname.emplace(gid, groupname);

    return true;
}

bool GroupTable::remove(const std::string& groupname) {
    auto it = groups.find(groupname);
    if (it == groups.end()) {
        return false;
    }

    groupId_t gid = it->second->getGid();

    groups.erase(it);
    id_to_groupname.erase(gid);

    return true;
}

IGroup* GroupTable::getByName(const std::string& groupname) const {
    auto it = groups.find(groupname);
    return it != groups.end() ? it->second.get() : nullptr;
}

IGroup *GroupTable::getById(groupId_t id) const
{
    auto it = id_to_groupname.find(id);
    if (it != id_to_groupname.end()) {
        return getByName(it->second);
    }
    return nullptr;
}

std::vector<const IGroup*> GroupTable::getItems() const {
    std::vector<const IGroup*> result;
    result.reserve(groups.size());
    for (const auto& it : groups) {
        result.push_back(it.second.get());
    }
    return result;
}

bool GroupTable::contains(const std::string& groupname) const {
    return groups.find(groupname) != groups.end();
}

bool GroupTable::contains(groupId_t id) const {
    return id_to_groupname.contains(id) &&
           contains(id_to_groupname.find(id)->second);
}

size_t GroupTable::size() const {
    return groups.size();
}

std::vector<std::string> GroupTable::listGroupnames() const {
    std::vector<std::string> result;
    result.reserve(groups.size());
    for (const auto& [groupname, _] : groups) {
        result.push_back(groupname);
    }
    return result;
}

void GroupTable::clear() {
    groups.clear();
    id_to_groupname.clear();
}
}