#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "mappers/entity/user_mapper.h"
#include "domain/entity/user.h"
#include "snapshots/entity/user_snapshot.h"

using namespace domain;

TEST_CASE("UserMapper toSnapshot basic") {
    UserMapper mapper;

    User user(1000, "testuser", 2000);
    auto snapshot = mapper.toSnapshot(user);

    REQUIRE(snapshot.getUid() == 1000);
    REQUIRE(snapshot.getName() == "testuser");
    REQUIRE(snapshot.getMainGroupId() == 2000);
    REQUIRE(snapshot.getSecondaryGroups().empty());
}

TEST_CASE("UserMapper toSnapshot with secondary groups") {
    UserMapper mapper;

    User user(1000, "testuser", 2000);
    user.addSecondaryGroup(3000);
    user.addSecondaryGroup(4000);

    auto snapshot = mapper.toSnapshot(user);

    REQUIRE(snapshot.getUid() == 1000);
    REQUIRE(snapshot.getSecondaryGroups().size() == 2);

    REQUIRE((snapshot.getSecondaryGroups()[0] == 3000 || snapshot.getSecondaryGroups()[0] == 4000));
    REQUIRE((snapshot.getSecondaryGroups()[1] == 3000 || snapshot.getSecondaryGroups()[1] == 4000));
    REQUIRE(snapshot.getSecondaryGroups()[0] != snapshot.getSecondaryGroups()[1]);
}

TEST_CASE("UserMapper fromSnapshot basic") {
    UserMapper mapper;

    UserSnapshot snapshot(1000, "testuser", 2000, {});
    auto user = mapper.fromSnapshot(snapshot);

    REQUIRE(user != nullptr);
    REQUIRE(user->getUid() == 1000);
    REQUIRE(user->getUsername() == "testuser");
    REQUIRE(user->getGroup() == 2000);
    REQUIRE(user->getSecondaryGroups().empty());
}

TEST_CASE("UserMapper fromSnapshot with secondary groups") {
    UserMapper mapper;

    std::vector<std::uint64_t> secondary_groups = {3000, 4000};
    UserSnapshot snapshot(1000, "testuser", 2000, secondary_groups);
    auto user = mapper.fromSnapshot(snapshot);

    REQUIRE(user != nullptr);
    REQUIRE(user->getUid() == 1000);
    REQUIRE(user->getUsername() == "testuser");
    REQUIRE(user->getGroup() == 2000);
    REQUIRE(user->getSecondaryGroups().size() == 2);
    REQUIRE(user->getSecondaryGroups().contains(3000));
    REQUIRE(user->getSecondaryGroups().contains(4000));
}

TEST_CASE("UserMapper roundtrip") {
    UserMapper mapper;

    User original_user(1000, "testuser", 2000);
    original_user.addSecondaryGroup(3000);
    original_user.addSecondaryGroup(4000);

    auto snapshot = mapper.toSnapshot(original_user);
    auto restored_user = mapper.fromSnapshot(snapshot);

    REQUIRE(restored_user->getUid() == 1000);
    REQUIRE(restored_user->getUsername() == "testuser");
    REQUIRE(restored_user->getGroup() == 2000);
    REQUIRE(restored_user->getSecondaryGroups().size() == 2);
}

