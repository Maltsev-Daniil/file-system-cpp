#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "factories/entity/user_fac.h"
#include "domain/entity/user.h"

using namespace domain;

TEST_CASE("UidGenerator generate") {
    UidGenerator gen;
    REQUIRE(gen.generate() == 1000);
    REQUIRE(gen.generate() == 1001);
    REQUIRE(gen.generate() == 1002);
}

TEST_CASE("UidGenerator advanceTo") {
    UidGenerator gen;
    gen.generate();
    gen.generate();
    REQUIRE(gen.generate() == 1002);

    gen.advanceTo(2000);
    REQUIRE(gen.generate() == 2000);
}

TEST_CASE("UidGenerator advanceTo smaller") {
    UidGenerator gen;
    gen.advanceTo(2000);
    gen.advanceTo(1500);
    REQUIRE(gen.generate() == 2000);
}

TEST_CASE("UserFactory createUser") {
    UidGenerator uid_gen;
    UserFactory factory(uid_gen);

    auto user = factory.createUser("testuser", 200);
    REQUIRE(user != nullptr);
    REQUIRE(user->getUsername() == "testuser");
    REQUIRE(user->getGroup() == 200);
    REQUIRE(user->getUid() >= 1000);
}

TEST_CASE("UserFactory createAdmin") {
    UidGenerator uid_gen;
    UserFactory factory(uid_gen);

    auto admin = factory.createAdmin();
    REQUIRE(admin != nullptr);
    REQUIRE(admin->getUid() == 0);
}

TEST_CASE("UserFactory generateId") {
    UidGenerator uid_gen;
    UserFactory factory(uid_gen);

    auto user = factory.createUser("testuser", 200);
    auto old_uid = user->getUid();

    factory.generateId(user.get());
    REQUIRE(user->getUid() > old_uid);
}

TEST_CASE("UserFactory getGen") {
    UidGenerator uid_gen;
    UserFactory factory(uid_gen);

    auto& gen = factory.getGen();
    REQUIRE(gen.generate() >= 1000);
}

TEST_CASE("UserFactory multiple users") {
    UidGenerator uid_gen;
    UserFactory factory(uid_gen);

    auto user1 = factory.createUser("user1", 200);
    auto user2 = factory.createUser("user2", 200);
    auto user3 = factory.createUser("user3", 200);

    REQUIRE(user1->getUid() != user2->getUid());
    REQUIRE(user2->getUid() != user3->getUid());
    REQUIRE(user1->getUid() != user3->getUid());
}

