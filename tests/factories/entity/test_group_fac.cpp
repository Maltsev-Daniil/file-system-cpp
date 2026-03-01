#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "factories/entity/group_fac.h"
#include "domain/entity/group.h"

using namespace domain;

TEST_CASE("GidGenerator generate") {
    GidGenerator gen;
    REQUIRE(gen.generate() == 1000);
    REQUIRE(gen.generate() == 1001);
    REQUIRE(gen.generate() == 1002);
}

TEST_CASE("GidGenerator advanceTo") {
    GidGenerator gen;
    gen.generate();
    gen.generate();
    REQUIRE(gen.generate() == 1002);

    gen.advanceTo(2000);
    REQUIRE(gen.generate() == 2000);
}

TEST_CASE("GidGenerator advanceTo smaller") {
    GidGenerator gen;
    gen.advanceTo(2000);
    gen.advanceTo(1500);
    REQUIRE(gen.generate() == 2000);
}

TEST_CASE("GroupFactory createGroup") {
    GidGenerator gid_gen;
    GroupFactory factory(gid_gen);

    auto group = factory.createGroup("testgroup");
    REQUIRE(group != nullptr);
    REQUIRE(group->getGroupname() == "testgroup");
    REQUIRE(group->getGid() >= 1000);
}

TEST_CASE("GroupFactory createAdminGroup") {
    GidGenerator gid_gen;
    GroupFactory factory(gid_gen);

    auto admin_group = factory.createAdminGroup();
    REQUIRE(admin_group != nullptr);
    REQUIRE(admin_group->getGid() == 0);
}

TEST_CASE("GroupFactory generateId") {
    GidGenerator gid_gen;
    GroupFactory factory(gid_gen);

    auto group = factory.createGroup("testgroup");
    auto old_gid = group->getGid();

    factory.generateId(group.get());
    REQUIRE(group->getGid() > old_gid);
}

TEST_CASE("GroupFactory getGen") {
    GidGenerator gid_gen;
    GroupFactory factory(gid_gen);

    auto& gen = factory.getGen();
    REQUIRE(gen.generate() >= 1000);
}

TEST_CASE("GroupFactory multiple groups") {
    GidGenerator gid_gen;
    GroupFactory factory(gid_gen);

    auto group1 = factory.createGroup("group1");
    auto group2 = factory.createGroup("group2");
    auto group3 = factory.createGroup("group3");

    REQUIRE(group1->getGid() != group2->getGid());
    REQUIRE(group2->getGid() != group3->getGid());
    REQUIRE(group1->getGid() != group3->getGid());
}

