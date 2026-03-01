#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "dto/rights/file_acl_dto.h"
#include "domain/objects/acl.h"

using namespace domain;

TEST_CASE("FileAclDTO basic creation") {
    FileAclDTO dto("/test/file.txt", "user1", "group1");
    REQUIRE(dto.file_path == "/test/file.txt");
    REQUIRE(dto.owner == "user1");
    REQUIRE(dto.group == "group1");
    REQUIRE(dto.entries.empty());
}

TEST_CASE("FileAclDTO addEntry") {
    FileAclDTO dto("/test/file.txt", "user1", "group1");
    std::set<ACLItem> perms = {
        ACLItem(RightType::READ, AccessType::ALLOW),
        ACLItem(RightType::WRITE, AccessType::ALLOW)
    };

    dto.addEntry("user", "user2", perms);
    REQUIRE(dto.entries.size() == 1);
    REQUIRE(dto.entries[0].type == "user");
    REQUIRE(dto.entries[0].name == "user2");
}

TEST_CASE("FileAclDTO multiple entries") {
    FileAclDTO dto("/test/file.txt", "user1", "group1");
    std::set<ACLItem> perms1 = {ACLItem(RightType::READ, AccessType::ALLOW)};
    std::set<ACLItem> perms2 = {ACLItem(RightType::WRITE, AccessType::ALLOW)};

    dto.addEntry("user", "user2", perms1);
    dto.addEntry("group", "group2", perms2);

    REQUIRE(dto.entries.size() == 2);
}

