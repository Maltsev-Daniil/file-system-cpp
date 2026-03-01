#include <string>
#include <vector>
#include <set>
#include "../../include/domain/types.h"
#include "../../include/domain/objects/acl.h"
#include "../../include/dto/rights/file_acl_dto.h"

using namespace domain;

void FileAclDTO::addEntry(const std::string &type, const std::string &name, const std::set<ACLItem> &perms)
{
    entries.push_back(
        SimpleAclEntry{
            type,
            name,
            permsToString(perms)});
}

std::string FileAclDTO::permsToString(const std::set<ACLItem> &perms)
{
    std::string result;

    for (const auto &item : perms)
    {
        // тк нужны эффективные права
        if (item.getType() != AccessType::ALLOW)
        {
            continue;
        }

        switch (item.getPerm())
        {
        case RightType::READ:
            result += 'r';
            break;
        case RightType::WRITE:
            result += 'w';
            break;
        case RightType::EXECUTE:
            result += 'x';
            break;
        default:
            break;
        }
    }

    return result;
}
