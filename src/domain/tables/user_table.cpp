//
// Created by nitsir on 12/25/25.
//

#include "domain/tables/user_table.h"

#include "domain/types.h"

namespace domain {
bool UserTable::add(std::unique_ptr<IUser> user) {
    if (!user) return false;

    std::string username = user->getUsername();
    userId_t uid = user->getUid();

    if (username.empty()) return false;
    if (users.count(username) || id_to_username.count(uid)) {
        return false;
    }

    users.emplace(username, std::move(user));
    id_to_username.emplace(uid, username);

    return true;
}

bool UserTable::remove(const std::string& username) {
    auto user_it = users.find(username);
    if (user_it == users.end()) {
        return false;
    }

    userId_t uid = user_it->second->getUid();

    users.erase(user_it);
    id_to_username.erase(uid);

    return true;
}

IUser* UserTable::getByName(const std::string& username) const {
    auto it = users.find(username);
    return it != users.end() ? it->second.get() : nullptr;
}

std::vector<const IUser*> UserTable::getItems() const {
    std::vector<const IUser*> result;
    result.reserve(users.size());
    for (const auto& it : users) {
        result.push_back(it.second.get());
    }
    return result;
}

bool UserTable::contains(const std::string& username) const {
    return users.find(username) != users.end();
}

bool UserTable::contains(userId_t id) const {
    return id_to_username.contains(id) &&
           contains(id_to_username.find(id)->second);
}

size_t UserTable::size() const {
    return users.size();
}

std::vector<std::string> UserTable::listUsernames() const {
    std::vector<std::string> result;
    result.reserve(users.size());
    for (const auto& [username, _] : users) {
        result.push_back(username);
    }
    return result;
}

void UserTable::clear() {
    users.clear();
    id_to_username.clear();
}

IUser *UserTable::getById(userId_t id) const
{
    auto it = id_to_username.find(id);
    if (it != id_to_username.end()) {
        return getByName(it->second);
    }
    return nullptr;
}
}
