#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/entity/group.h"

using namespace domain;

TEST_CASE("Group basic creation") {
    Group group(100, "testgroup");
    REQUIRE(group.getGid() == 100);
    REQUIRE(group.getGroupname() == "testgroup");
}

TEST_CASE("Group setGid") {
    Group group(100, "testgroup");
    group.setGid(101);
    REQUIRE(group.getGid() == 101);
}

TEST_CASE("Group setGroupname") {
    Group group(100, "testgroup");
    group.setGroupname("newgroup");
    REQUIRE(group.getGroupname() == "newgroup");
}

TEST_CASE("Group addUser") {
    Group group(100, "testgroup");
    group.addUser(1);
    REQUIRE(group.getUsers().size() == 1);
    REQUIRE(group.getUsers().count(1) == 1);
}

TEST_CASE("Group delUser") {
    Group group(100, "testgroup");
    group.addUser(1);
    group.addUser(2);
    REQUIRE(group.getUsers().size() == 2);

    group.delUser(1);
    REQUIRE(group.getUsers().size() == 1);
    REQUIRE(group.getUsers().count(1) == 0);
    REQUIRE(group.getUsers().count(2) == 1);
}

TEST_CASE("Group addGroup") {
    Group group(100, "testgroup");
    group.addGroup(201);
    REQUIRE(group.getSubGroups().size() == 1);
    REQUIRE(group.getSubGroups().count(201) == 1);
}

TEST_CASE("Group delGroup") {
    Group group(100, "testgroup");
    group.addGroup(201);
    group.addGroup(202);
    REQUIRE(group.getSubGroups().size() == 2);

    group.delGroup(201);
    REQUIRE(group.getSubGroups().size() == 1);
    REQUIRE(group.getSubGroups().count(201) == 0);
    REQUIRE(group.getSubGroups().count(202) == 1);
}

TEST_CASE("Group multiple users") {
    Group group(100, "testgroup");
    group.addUser(1);
    group.addUser(2);
    group.addUser(3);

    REQUIRE(group.getUsers().size() == 3);
    REQUIRE(group.getUsers().count(1) == 1);
    REQUIRE(group.getUsers().count(2) == 1);
    REQUIRE(group.getUsers().count(3) == 1);
}

TEST_CASE("Group multiple subgroups") {
    Group group(100, "testgroup");
    group.addGroup(201);
    group.addGroup(202);
    group.addGroup(203);

    REQUIRE(group.getSubGroups().size() == 3);
    REQUIRE(group.getSubGroups().count(201) == 1);
    REQUIRE(group.getSubGroups().count(202) == 1);
    REQUIRE(group.getSubGroups().count(203) == 1);
}

TEST_CASE("Group del non-existent user") {
    Group group(100, "testgroup");
    group.addUser(1);
    group.delUser(999);

    REQUIRE(group.getUsers().size() == 1);
    REQUIRE(group.getUsers().count(1) == 1);
}

TEST_CASE("Group del non-existent subgroup") {
    Group group(100, "testgroup");
    group.addGroup(201);
    group.delGroup(999);

    REQUIRE(group.getSubGroups().size() == 1);
    REQUIRE(group.getSubGroups().count(201) == 1);
}

TEST_CASE("Group clone") {
    Group group(100, "testgroup");
    group.addUser(1);
    group.addUser(2);
    group.addGroup(201);
    group.addGroup(202);

    auto cloned = group.clone();
    REQUIRE(cloned->getGid() == 100);
    REQUIRE(cloned->getGroupname() == "testgroup");
    REQUIRE(cloned->getUsers().size() == 2);
    REQUIRE(cloned->getSubGroups().size() == 2);
}

TEST_CASE("Group copy constructor") {
    Group group(100, "testgroup");
    group.addUser(1);
    group.addGroup(201);

    Group copied(group);
    REQUIRE(copied.getGid() == 100);
    REQUIRE(copied.getGroupname() == "testgroup");
    REQUIRE(copied.getUsers().size() == 1);
    REQUIRE(copied.getSubGroups().size() == 1);
}

TEST_CASE("Group empty collections") {
    Group group(100, "testgroup");
    REQUIRE(group.getUsers().empty());
    REQUIRE(group.getSubGroups().empty());
}

