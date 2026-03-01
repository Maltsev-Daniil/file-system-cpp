#pragma once
#include <unordered_map>

#include "itable.h"
#include "../entity/user.h"

namespace domain {

class UserTable : public IUserTable {
private:
    std::unordered_map<std::string, std::unique_ptr<IUser>> users;
    std::unordered_map<userId_t, std::string> id_to_username;

public:
    bool add(std::unique_ptr<IUser> user) override;
    bool remove(const std::string& username) override;
    IUser* getByName(const std::string& username) const override;
    IUser* getById(userId_t id) const override;
    std::vector<const IUser*> getItems() const override;

    bool contains(const std::string& username) const override;
    bool contains(userId_t id) const override;
    size_t size() const override;

    std::vector<std::string> listUsernames() const;
    void clear();
};
}
