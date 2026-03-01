#pragma once

#include <string>
#include <vector>

enum class UsermodOpt {
    CH_MAIN_GROUP,
    ADD_SECONDARY_GROUPS
};

struct UsermodRequest {
    UsermodOpt kind;
    std::vector<std::string> groups;
};