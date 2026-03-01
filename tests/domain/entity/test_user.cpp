#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/entity/user.h"

using namespace domain;

TEST_CASE("User basic creation") {
    User user(100, "testuser", 200);
    REQUIRE(user.getUid() == 100);
    REQUIRE(user.getUsername() == "testuser");
    REQUIRE(user.getGroup() == 200);
}

TEST_CASE("User with secondary groups") {
    std::unordered_set<groupId_t> sec_groups = {201, 202, 203};
    User user(100, "testuser", 200, sec_groups);

    REQUIRE(user.getUid() == 100);
    REQUIRE(user.getGroup() == 200);
    REQUIRE(user.getSecondaryGroups().size() == 3);
    REQUIRE(user.getSecondaryGroups().count(201) == 1);
    REQUIRE(user.getSecondaryGroups().count(202) == 1);
    REQUIRE(user.getSecondaryGroups().count(203) == 1);
}

TEST_CASE("User setUid") {
    User user(100, "testuser", 200);
    user.setUid(101);
    REQUIRE(user.getUid() == 101);
}

TEST_CASE("User setUsername") {
    User user(100, "testuser", 200);
    user.setUsername("newuser");
    REQUIRE(user.getUsername() == "newuser");
}

TEST_CASE("User setGroup") {
    User user(100, "testuser", 200);
    user.setGroup(201);
    REQUIRE(user.getGroup() == 201);
}

TEST_CASE("User addSecondaryGroup") {
    User user(100, "testuser", 200);
    user.addSecondaryGroup(201);
    REQUIRE(user.getSecondaryGroups().size() == 1);
    REQUIRE(user.getSecondaryGroups().count(201) == 1);
}

TEST_CASE("User removeSecondaryGroup") {
    User user(100, "testuser", 200);
    user.addSecondaryGroup(201);
    user.addSecondaryGroup(202);
    REQUIRE(user.getSecondaryGroups().size() == 2);

    user.removeSecondaryGroup(201);
    REQUIRE(user.getSecondaryGroups().size() == 1);
    REQUIRE(user.getSecondaryGroups().count(201) == 0);
    REQUIRE(user.getSecondaryGroups().count(202) == 1);
}

TEST_CASE("User multiple secondary groups") {
    User user(100, "testuser", 200);
    user.addSecondaryGroup(201);
    user.addSecondaryGroup(202);
    user.addSecondaryGroup(203);

    REQUIRE(user.getSecondaryGroups().size() == 3);
    REQUIRE(user.getSecondaryGroups().count(201) == 1);
    REQUIRE(user.getSecondaryGroups().count(202) == 1);
    REQUIRE(user.getSecondaryGroups().count(203) == 1);
}

TEST_CASE("User remove non-existent secondary group") {
    User user(100, "testuser", 200);
    user.addSecondaryGroup(201);
    user.removeSecondaryGroup(999);

    REQUIRE(user.getSecondaryGroups().size() == 1);
    REQUIRE(user.getSecondaryGroups().count(201) == 1);
}

TEST_CASE("User clone") {
    User user(100, "testuser", 200);
    user.addSecondaryGroup(201);
    user.addSecondaryGroup(202);

    auto cloned = user.clone();
    REQUIRE(cloned->getUid() == 100);
    REQUIRE(cloned->getUsername() == "testuser");
    REQUIRE(cloned->getGroup() == 200);
    REQUIRE(cloned->getSecondaryGroups().size() == 2);
    REQUIRE(cloned->getSecondaryGroups().count(201) == 1);
    REQUIRE(cloned->getSecondaryGroups().count(202) == 1);
}

TEST_CASE("User empty secondary groups") {
    User user(100, "testuser", 200);
    REQUIRE(user.getSecondaryGroups().empty());
}

