#include <cctype>
#include "../../include/alias/alias_registry.h"

#include <stdexcept>
#include "../../include/alias/alias_handler.h"
#include <sstream>

void AliasRegistry::registerAlias(
    const std::vector<std::string>& tokens_bad
) {
    if (tokens_bad.size() < 2) {
        throw std::logic_error("alias: invalid syntax");
    }

    std::string joined_without_alias;
    for (size_t i = 1; i < tokens_bad.size(); ++i) {
        if (i > 1) joined_without_alias += " ";
        joined_without_alias += tokens_bad[i];
    }

    const auto name = extractAliasName(joined_without_alias);
    const auto expansion =
        parseExpansion(joined_without_alias);

    if (alias_handler_.exists(name)) {
        alias_handler_.update(name, expansion);
        return;
    }

    alias_handler_.add(name, expansion);
}

std::vector<std::string>
AliasRegistry::expandRecursive(
    const std::string& line,
    std::unordered_set<std::string>& visited
) const {
    std::istringstream iss(line);
    std::string first;
    iss >> first;

    if (!alias_handler_.exists(first)) {
        return { line };
    }

    if (visited.contains(first)) {
        throw std::logic_error(
            "alias expansion cycle detected: " + first
        );
    }

    visited.insert(first);

    const auto& expansion = alias_handler_.get(first);

    std::vector<std::string> result;
    for (const auto& cmd : expansion) {
        auto expanded = expandRecursive(cmd, visited);
        result.insert(
            result.end(),
            expanded.begin(),
            expanded.end()
        );
    }

    visited.erase(first);
    return result;
}

std::vector<std::string>
AliasRegistry::expandIfAlias(
    const std::string& line
) const {
    if (line.empty()) {
        return {};
    }

    std::unordered_set<std::string> visited;
    return expandRecursive(line, visited);
}

std::string
AliasRegistry::extractAliasName(
    const std::string& token
) {
    auto pos = token.find('=');
    if (pos == std::string::npos) {
        throw std::logic_error("alias: missing '='");
    }

    return token.substr(0, pos);
}

std::vector<std::string>
AliasRegistry::parseExpansion(
    const std::string& raw
) {
    auto pos = raw.find('=');
    if (pos == std::string::npos) {
        throw std::logic_error("alias: missing '='");
    }

    auto rhs = stripQuotes(
        raw.substr(pos + 1)
    );

    return splitCommands(rhs);
}

std::string
AliasRegistry::stripQuotes(
    const std::string& s
) {
    if (s.size() >= 2 &&
        ((s.front() == '\'' && s.back() == '\'') ||
         (s.front() == '"'  && s.back() == '"'))) {
        return s.substr(1, s.size() - 2);
    }

    return s;
}

std::vector<std::string>
AliasRegistry::splitCommands(
    const std::string& s
) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string part;

    while (std::getline(ss, part, ';')) {
        auto t = trim(part);
        if (!t.empty()) {
            result.push_back(t);
        }
    }

    return result;
}

std::string
AliasRegistry::trim(
    const std::string& s
) {
    size_t l = 0;
    size_t r = s.size();

    while (l < r && std::isspace(s[l])) ++l;
    while (r > l && std::isspace(s[r - 1])) --r;

    return s.substr(l, r - l);
}