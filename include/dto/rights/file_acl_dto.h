#pragma once

#include <string>
#include <vector>
#include <set>

#include "domain/objects/acl.h"

struct SimpleAclEntry {
    std::string type;
    std::string name;
    std::string perms;
};

class FileAclDTO {
public:
    std::string file_path;
    std::string owner;
    std::string group;
    std::vector<SimpleAclEntry> entries;

    FileAclDTO(const std::string& path,
               std::string owner,
               std::string group)
        : file_path(path)
        , owner(std::move(owner))
        , group(std::move(group))
    {}

    void addEntry(
        const std::string& type,
        const std::string& name,
        const std::set<domain::ACLItem>& perms);

private:
    static std::string permsToString(const std::set<domain::ACLItem>& perms);
};
