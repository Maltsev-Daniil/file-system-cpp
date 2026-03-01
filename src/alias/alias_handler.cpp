
#include "alias/alias_handler.h"

#include <stdexcept>
#include <vector>

void AliasHandler::add(
    const std::string& name,
    std::vector<std::string> commands
) {
    aliases.emplace(name, std::move(commands));
}

void AliasHandler::update(
    const std::string& name,
    std::vector<std::string> commands
) {
    auto it = aliases.find(name);
    if (it == aliases.end()) {
        throw std::logic_error("alias update: alias not found");
    }

    it->second = std::move(commands);
}

void AliasHandler::remove(
    const std::string& name
) {
    aliases.erase(name);
}

bool AliasHandler::exists(
    const std::string& name
) const {
    return aliases.contains(name);
}

const std::vector<std::string>& AliasHandler::get(
    const std::string& name
) const {
    auto it = aliases.find(name);
    if (it == aliases.end()) {
        throw std::logic_error("alias get: alias not found");
    }

    return it->second;
}