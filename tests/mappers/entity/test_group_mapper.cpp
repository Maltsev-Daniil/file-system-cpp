#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "mappers/entity/group_mapper.h"
#include "domain/entity/group.h"
#include "snapshots/entity/group_snapshot.h"

using namespace domain;

TEST_CASE("GroupMapper toSnapshot basic") {
    GroupMapper mapper;

    Group group(1000, "testgroup");
    auto snapshot = mapper.toSnapshot(group);

    REQUIRE(snapshot.getGid() == 1000);
    REQUIRE(snapshot.getName() == "testgroup");
    REQUIRE(snapshot.getUsers().empty());
    REQUIRE(snapshot.getSubGroups().empty());
}

TEST_CASE("GroupMapper toSnapshot with users and subgroups") {
    GroupMapper mapper;

    Group group(1000, "testgroup");
    group.addUser(2000);
    group.addUser(3000);
    group.addGroup(4000);
    group.addGroup(5000);

    auto snapshot = mapper.toSnapshot(group);

    REQUIRE(snapshot.getGid() == 1000);
    REQUIRE(snapshot.getUsers().size() == 2);
    REQUIRE(snapshot.getSubGroups().size() == 2);
}

TEST_CASE("GroupMapper fromSnapshot basic") {
    GroupMapper mapper;

    GroupSnapshot snapshot(1000, "testgroup", {}, {});
    auto group = mapper.fromSnapshot(snapshot);

    REQUIRE(group != nullptr);
    REQUIRE(group->getGid() == 1000);
    REQUIRE(group->getGroupname() == "testgroup");
    REQUIRE(group->getUsers().empty());
    REQUIRE(group->getSubGroups().empty());
}

TEST_CASE("GroupMapper fromSnapshot with users and subgroups") {
    GroupMapper mapper;

    std::vector<std::uint64_t> users = {2000, 3000};
    std::vector<std::uint64_t> subgroups = {4000, 5000};
    GroupSnapshot snapshot(1000, "testgroup", users, subgroups);
    auto group = mapper.fromSnapshot(snapshot);

    REQUIRE(group != nullptr);
    REQUIRE(group->getGid() == 1000);
    REQUIRE(group->getGroupname() == "testgroup");
    REQUIRE(group->getUsers().size() == 2);
    REQUIRE(group->getSubGroups().size() == 2);
    REQUIRE(group->getUsers().contains(2000));
    REQUIRE(group->getUsers().contains(3000));
    REQUIRE(group->getSubGroups().contains(4000));
    REQUIRE(group->getSubGroups().contains(5000));
}

TEST_CASE("GroupMapper roundtrip") {
    GroupMapper mapper;

    Group original_group(1000, "testgroup");
    original_group.addUser(2000);
    original_group.addGroup(4000);

    auto snapshot = mapper.toSnapshot(original_group);
    auto restored_group = mapper.fromSnapshot(snapshot);

    REQUIRE(restored_group->getGid() == 1000);
    REQUIRE(restored_group->getGroupname() == "testgroup");
    REQUIRE(restored_group->getUsers().size() == 1);
    REQUIRE(restored_group->getSubGroups().size() == 1);
}

