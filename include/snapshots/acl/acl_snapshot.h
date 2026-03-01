#pragma once

#include "acl_item_snapshot.h"
#include <vector>


class AclSnapshot {
public:
    AclSnapshot(std::vector<AclItemSnapshot> items)
        : items_(std::move(items)) {};

    const std::vector<AclItemSnapshot>& getItems() const noexcept { return items_; };

private:
    std::vector<AclItemSnapshot> items_;
};