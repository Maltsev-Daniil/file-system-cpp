#pragma once

#include <string>
#include <vector>

enum class GroupmodOpt {
    RENAME,
    ADD_GROUP,
    ADD_USER,
    REMOVE_GROUP,
    REMOVE_USER
};

struct GroupmodRequest {
    GroupmodOpt kind;
    std::vector<std::string> args;
};