#pragma once

#include <vector>
#include <string>

struct CmdletRequest {
    std::vector<std::string> flags;
    std::vector<std::string> args;
};