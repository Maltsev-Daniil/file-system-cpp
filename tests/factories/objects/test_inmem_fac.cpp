#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "domain/objects/inmem_file.h"

using namespace domain;

TEST_CASE("InodeGenerator generate") {
    InodeGenerator gen;
    REQUIRE(gen.generate() == 1);
    REQUIRE(gen.generate() == 2);
    REQUIRE(gen.generate() == 3);
}

TEST_CASE("InodeGenerator advanceTo") {
    InodeGenerator gen;
    gen.generate();
    gen.generate();
    REQUIRE(gen.generate() == 3);

    gen.advanceTo(100);
    REQUIRE(gen.generate() == 100);
}

TEST_CASE("InodeGenerator advanceTo smaller") {
    InodeGenerator gen;
    gen.advanceTo(100);
    gen.advanceTo(50);
    REQUIRE(gen.generate() == 100);
}

TEST_CASE("InMemoryFactory createInMemory") {
    InodeGenerator inode_gen;
    InMemoryFactory factory(inode_gen);

    auto inmem = factory.createInMemory(100, 200);
    REQUIRE(inmem != nullptr);
    REQUIRE(inmem->getUid() == 100);
    REQUIRE(inmem->getGid() == 200);
    REQUIRE(inmem->getId() >= 1);
}

TEST_CASE("InMemoryFactory createInMemory with ACL") {
    InodeGenerator inode_gen;
    InMemoryFactory factory(inode_gen);
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();

    auto inmem = factory.createInMemory(100, 200, std::move(acl));
    REQUIRE(inmem != nullptr);
    REQUIRE(inmem->getAcl() != nullptr);
}

TEST_CASE("InMemoryFactory createFullInMemory") {
    InodeGenerator inode_gen;
    InMemoryFactory factory(inode_gen);
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();

    auto now = std::chrono::system_clock::now();
    auto inmem = factory.createFullInMemory(999, 100, 200, 1024, std::move(acl), now, now);
    REQUIRE(inmem != nullptr);
    REQUIRE(inmem->getId() == 999);
    REQUIRE(inmem->getUid() == 100);
    REQUIRE(inmem->getGid() == 200);
    REQUIRE(inmem->getSize() == 1024);
}

TEST_CASE("InMemoryFactory generateId") {
    InodeGenerator inode_gen;
    InMemoryFactory factory(inode_gen);

    auto inmem = factory.createInMemory(100, 200);
    auto old_id = inmem->getId();

    factory.generateId(inmem.get());
    REQUIRE(inmem->getId() > old_id);
}

TEST_CASE("InMemoryFactory getGen") {
    InodeGenerator inode_gen;
    InMemoryFactory factory(inode_gen);

    auto& gen = factory.getGen();
    REQUIRE(gen.generate() >= 1);
}

TEST_CASE("InMemoryFactory multiple inodes") {
    InodeGenerator inode_gen;
    InMemoryFactory factory(inode_gen);

    auto inmem1 = factory.createInMemory(100, 200);
    auto inmem2 = factory.createInMemory(100, 200);
    auto inmem3 = factory.createInMemory(100, 200);

    REQUIRE(inmem1->getId() != inmem2->getId());
    REQUIRE(inmem2->getId() != inmem3->getId());
    REQUIRE(inmem1->getId() != inmem3->getId());
}

