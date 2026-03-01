#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/tables/user_table.h"
#include "domain/entity/user.h"

using namespace domain;

std::unique_ptr<IUser> createTestUser(userId_t uid, const std::string& name, groupId_t gid) {
    return std::make_unique<User>(uid, name, gid);
}

TEST_CASE("UserTable add") {
    UserTable table;
    auto user = createTestUser(100, "testuser", 200);
    REQUIRE(table.add(std::move(user)));
    REQUIRE(table.size() == 1);
}

TEST_CASE("UserTable contains by username") {
    UserTable table;
    auto user = createTestUser(100, "testuser", 200);
    table.add(std::move(user));

    REQUIRE(table.contains("testuser"));
    REQUIRE_FALSE(table.contains("nonexistent"));
}

TEST_CASE("UserTable contains by id") {
    UserTable table;
    auto user = createTestUser(100, "testuser", 200);
    table.add(std::move(user));

    REQUIRE(table.contains(100));
    REQUIRE_FALSE(table.contains(999));
}

TEST_CASE("UserTable getByName") {
    UserTable table;
    auto user = createTestUser(100, "testuser", 200);
    table.add(std::move(user));

    auto* retrieved = table.getByName("testuser");
    REQUIRE(retrieved != nullptr);
    REQUIRE(retrieved->getUid() == 100);
    REQUIRE(retrieved->getUsername() == "testuser");
}

TEST_CASE("UserTable getById") {
    UserTable table;
    auto user = createTestUser(100, "testuser", 200);
    table.add(std::move(user));

    auto* retrieved = table.getById(100);
    REQUIRE(retrieved != nullptr);
    REQUIRE(retrieved->getUid() == 100);
    REQUIRE(retrieved->getUsername() == "testuser");
}

TEST_CASE("UserTable remove") {
    UserTable table;
    auto user = createTestUser(100, "testuser", 200);
    table.add(std::move(user));
    REQUIRE(table.size() == 1);

    REQUIRE(table.remove("testuser"));
    REQUIRE(table.size() == 0);
    REQUIRE_FALSE(table.contains("testuser"));
}

TEST_CASE("UserTable remove non-existent") {
    UserTable table;
    REQUIRE_FALSE(table.remove("nonexistent"));
}

TEST_CASE("UserTable size") {
    UserTable table;
    REQUIRE(table.size() == 0);

    table.add(createTestUser(100, "user1", 200));
    REQUIRE(table.size() == 1);

    table.add(createTestUser(101, "user2", 200));
    REQUIRE(table.size() == 2);
}

TEST_CASE("UserTable getItems") {
    UserTable table;
    table.add(createTestUser(100, "user1", 200));
    table.add(createTestUser(101, "user2", 200));

    auto items = table.getItems();
    REQUIRE(items.size() == 2);
}

TEST_CASE("UserTable listUsernames") {
    UserTable table;
    table.add(createTestUser(100, "user1", 200));
    table.add(createTestUser(101, "user2", 200));

    auto names = table.listUsernames();
    REQUIRE(names.size() == 2);
    REQUIRE(std::find(names.begin(), names.end(), "user1") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "user2") != names.end());
}

TEST_CASE("UserTable clear") {
    UserTable table;
    table.add(createTestUser(100, "user1", 200));
    table.add(createTestUser(101, "user2", 200));
    REQUIRE(table.size() == 2);

    table.clear();
    REQUIRE(table.size() == 0);
}

TEST_CASE("UserTable duplicate add") {
    UserTable table;
    auto user1 = createTestUser(100, "testuser", 200);
    REQUIRE(table.add(std::move(user1)));

    auto user2 = createTestUser(100, "testuser", 200);
    REQUIRE_FALSE(table.add(std::move(user2)));
    REQUIRE(table.size() == 1);
}

TEST_CASE("UserTable multiple users") {
    UserTable table;
    table.add(createTestUser(100, "user1", 200));
    table.add(createTestUser(101, "user2", 201));
    table.add(createTestUser(102, "user3", 202));

    REQUIRE(table.size() == 3);
    REQUIRE(table.contains("user1"));
    REQUIRE(table.contains("user2"));
    REQUIRE(table.contains("user3"));
    REQUIRE(table.contains(100));
    REQUIRE(table.contains(101));
    REQUIRE(table.contains(102));
}

