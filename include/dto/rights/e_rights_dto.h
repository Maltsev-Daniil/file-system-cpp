#pragma once

#include "domain/types.h"
#include <set>

struct FileERightsDTO {
    std::set<domain::RightType> e_rights;
    FileERightsDTO(
    std::set<domain::RightType> e_rights) :
        e_rights(std::move(e_rights)) {}
};