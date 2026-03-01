#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "dto/objects/stat_file_dto.h"
#include "domain/objects/inmem_file.h"
#include "factories/objects/acl_fac.h"

using namespace domain;

TEST_CASE("StatFileDTO creation from InMemoryFile") {
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();
    InMemoryFile inmem(1, 100, 200, 1024, std::move(acl));

    StatFileDTO dto(&inmem);
    REQUIRE(dto.id == 1);
    REQUIRE(dto.uid == 100);
    REQUIRE(dto.gid == 200);
    REQUIRE(dto.size == 1024);
}

TEST_CASE("StatFileDTO time points") {
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();
    InMemoryFile inmem(1, 100, 200, 1024, std::move(acl));

    StatFileDTO dto(&inmem);
    REQUIRE(dto.creation_time == inmem.getCreateTime());
    REQUIRE(dto.modification_time == inmem.getModTime());
}

