#include "../include/processor.h"
#include <sstream>
#include "../include/cmd_dispatcher.h"
#include "requests/cmdlet/req_cmdlet.h"
#include <algorithm>

namespace view {
void CmdProcessor::process(const std::string& input_line, ICliView& view) {
    std::vector<std::string> tokens = tokenize(input_line);
    if (tokens[0] == "alias") {
        alias_registry_.registerAlias(tokens);
        return;
    }

    auto commands = alias_registry_.expandIfAlias(input_line);
    for (const auto& command : commands) {
        std::vector<std::string> tokens = tokenize(command);
        auto [cmdlet_name, req] = parseTokens(tokens);
        auto res = dispatcher.executeCmdlet(cmdlet_name, req);
        res->render(view);
    }
}

std::pair<std::string, CmdletRequest> CmdProcessor::parseTokens(const std::vector<std::string> &tokens)
{
    if (tokens.empty()) {
        return {"", CmdletRequest{}};
    }

    std::string name = tokens[0];
    CmdletRequest req;

    size_t pos = 1;
    while (tokens.size() > pos &&
            tokens[pos].size() > 0 &&
            tokens[pos].at(0) == '-') {
        const std::string& token = tokens[pos];
        // -abc
        if (token.size() > 1 && token.at(1) != '-') {
            for (size_t i = 1; i < token.size(); i++) 
                req.flags.emplace_back(1, token[i]);
        } else {
            // --flag -f
            req.flags.push_back(token);
        }
        pos++;
    } 
    std::copy(tokens.begin() + pos, tokens.end(),
         std::back_inserter(req.args));
    return std::make_pair(name, req);
}

std::vector<std::string> CmdProcessor::tokenize(const std::string& line) {
    std::istringstream ss(line);
    std::vector<std::string> tokens;
    std::string word;
    while (ss >> word) 
        tokens.push_back(std::move(word));
    return tokens;
}

}