#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/tables/group_table.h"
#include "domain/entity/group.h"

using namespace domain;

std::unique_ptr<IGroup> createTestGroup(groupId_t gid, const std::string& name) {
    return std::make_unique<Group>(gid, name);
}

TEST_CASE("GroupTable add") {
    GroupTable table;
    auto group = createTestGroup(100, "testgroup");
    REQUIRE(table.add(std::move(group)));
    REQUIRE(table.size() == 1);
}

TEST_CASE("GroupTable contains by groupname") {
    GroupTable table;
    auto group = createTestGroup(100, "testgroup");
    table.add(std::move(group));

    REQUIRE(table.contains("testgroup"));
    REQUIRE_FALSE(table.contains("nonexistent"));
}

TEST_CASE("GroupTable contains by id") {
    GroupTable table;
    auto group = createTestGroup(100, "testgroup");
    table.add(std::move(group));

    REQUIRE(table.contains(100));
    REQUIRE_FALSE(table.contains(999));
}

TEST_CASE("GroupTable getByName") {
    GroupTable table;
    auto group = createTestGroup(100, "testgroup");
    table.add(std::move(group));

    auto* retrieved = table.getByName("testgroup");
    REQUIRE(retrieved != nullptr);
    REQUIRE(retrieved->getGid() == 100);
    REQUIRE(retrieved->getGroupname() == "testgroup");
}

TEST_CASE("GroupTable getById") {
    GroupTable table;
    auto group = createTestGroup(100, "testgroup");
    table.add(std::move(group));

    auto* retrieved = table.getById(100);
    REQUIRE(retrieved != nullptr);
    REQUIRE(retrieved->getGid() == 100);
    REQUIRE(retrieved->getGroupname() == "testgroup");
}

TEST_CASE("GroupTable remove") {
    GroupTable table;
    auto group = createTestGroup(100, "testgroup");
    table.add(std::move(group));
    REQUIRE(table.size() == 1);

    REQUIRE(table.remove("testgroup"));
    REQUIRE(table.size() == 0);
    REQUIRE_FALSE(table.contains("testgroup"));
}

TEST_CASE("GroupTable remove non-existent") {
    GroupTable table;
    REQUIRE_FALSE(table.remove("nonexistent"));
}

TEST_CASE("GroupTable size") {
    GroupTable table;
    REQUIRE(table.size() == 0);

    table.add(createTestGroup(100, "group1"));
    REQUIRE(table.size() == 1);

    table.add(createTestGroup(101, "group2"));
    REQUIRE(table.size() == 2);
}

TEST_CASE("GroupTable getItems") {
    GroupTable table;
    table.add(createTestGroup(100, "group1"));
    table.add(createTestGroup(101, "group2"));

    auto items = table.getItems();
    REQUIRE(items.size() == 2);
}

TEST_CASE("GroupTable listGroupnames") {
    GroupTable table;
    table.add(createTestGroup(100, "group1"));
    table.add(createTestGroup(101, "group2"));

    auto names = table.listGroupnames();
    REQUIRE(names.size() == 2);
    REQUIRE(std::find(names.begin(), names.end(), "group1") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "group2") != names.end());
}

TEST_CASE("GroupTable clear") {
    GroupTable table;
    table.add(createTestGroup(100, "group1"));
    table.add(createTestGroup(101, "group2"));
    REQUIRE(table.size() == 2);

    table.clear();
    REQUIRE(table.size() == 0);
}

TEST_CASE("GroupTable duplicate add") {
    GroupTable table;
    auto group1 = createTestGroup(100, "testgroup");
    REQUIRE(table.add(std::move(group1)));

    auto group2 = createTestGroup(100, "testgroup");
    REQUIRE_FALSE(table.add(std::move(group2)));
    REQUIRE(table.size() == 1);
}

TEST_CASE("GroupTable multiple groups") {
    GroupTable table;
    table.add(createTestGroup(100, "group1"));
    table.add(createTestGroup(101, "group2"));
    table.add(createTestGroup(102, "group3"));

    REQUIRE(table.size() == 3);
    REQUIRE(table.contains("group1"));
    REQUIRE(table.contains("group2"));
    REQUIRE(table.contains("group3"));
    REQUIRE(table.contains(100));
    REQUIRE(table.contains(101));
    REQUIRE(table.contains(102));
}

