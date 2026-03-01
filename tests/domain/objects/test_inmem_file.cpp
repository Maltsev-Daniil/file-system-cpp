#define CATCH_CONFIG_MAIN
#include <thread>
#include <catch2/catch_all.hpp>

#include "domain/objects/inmem_file.h"
#include "factories/objects/acl_fac.h"

using namespace domain;

TEST_CASE("InMemoryFile basic creation") {
    InMemoryFile file(1, 100, 200, 1024);
    REQUIRE(file.getId() == 1);
    REQUIRE(file.getUid() == 100);
    REQUIRE(file.getGid() == 200);
    REQUIRE(file.getSize() == 1024);
}

TEST_CASE("InMemoryFile setUid") {
    InMemoryFile file(1);
    file.setUid(100);
    REQUIRE(file.getUid() == 100);
}

TEST_CASE("InMemoryFile setGid") {
    InMemoryFile file(1);
    file.setGid(200);
    REQUIRE(file.getGid() == 200);
}

TEST_CASE("InMemoryFile setSize") {
    InMemoryFile file(1);
    file.setSize(2048);
    REQUIRE(file.getSize() == 2048);
}

TEST_CASE("InMemoryFile setId") {
    InMemoryFile file(1);
    file.setId(999);
    REQUIRE(file.getId() == 999);
}

TEST_CASE("InMemoryFile getModTime") {
    auto before = std::chrono::system_clock::now();
    InMemoryFile file(1);
    auto after = std::chrono::system_clock::now();

    auto mod_time = file.getModTime();
    REQUIRE(mod_time >= before);
    REQUIRE(mod_time <= after);
}

TEST_CASE("InMemoryFile getCreateTime") {
    auto before = std::chrono::system_clock::now();
    InMemoryFile file(1);
    auto after = std::chrono::system_clock::now();

    auto create_time = file.getCreateTime();
    REQUIRE(create_time >= before);
    REQUIRE(create_time <= after);
}

TEST_CASE("InMemoryFile updateModificationTime") {
    InMemoryFile file(1);
    auto old_time = file.getModTime();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    file.updateModificationTime();

    auto new_time = file.getModTime();
    REQUIRE(new_time > old_time);
}

TEST_CASE("InMemoryFile with ACL") {
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();
    InMemoryFile file(1, 0, 0, 0, std::move(acl));

    REQUIRE(file.getAcl() != nullptr);
}

TEST_CASE("InMemoryFile getAcl const") {
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();
    InMemoryFile file(1, 0, 0, 0, std::move(acl));

    const InMemoryFile& cfile = file;
    REQUIRE(cfile.getAcl() != nullptr);
}

TEST_CASE("InMemoryFile clone") {
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();
    InMemoryFile file(1, 100, 200, 1024, std::move(acl));

    auto cloned = file.clone();
    REQUIRE(cloned->getId() == 1);
    REQUIRE(cloned->getUid() == 100);
    REQUIRE(cloned->getGid() == 200);
    REQUIRE(cloned->getSize() == 1024);
    REQUIRE(cloned->getAcl() != nullptr);
}

TEST_CASE("InMemoryFile default values") {
    InMemoryFile file(1);
    REQUIRE(file.getUid() == 0);
    REQUIRE(file.getGid() == 0);
    REQUIRE(file.getSize() == 0);
}

