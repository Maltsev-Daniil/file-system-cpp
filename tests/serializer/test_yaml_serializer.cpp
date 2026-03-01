#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "serializer/yaml_serializer.h"
#include "snapshots/file_system/file_system_snapshot.h"
#include "snapshots/entity/user_snapshot.h"
#include "snapshots/entity/group_snapshot.h"
#include "snapshots/objects/directory_snapshot.h"
#include "snapshots/objects/regular_file_snapshot.h"
#include "snapshots/objects/file_metadata_snapshot.h"
#include "snapshots/acl/acl_snapshot.h"
#include "snapshots/acl/acl_item_snapshot.h"
#include "snapshots/acl/acl_subject_snapshot.h"
#include <filesystem>
#include <fstream>

TEST_CASE("YamlFileSystemSerializer write and read FileSystemSnapshot") {
    YamlFileSystemSerializer serializer;


    std::vector<AclItemSnapshot> acl_items;
    acl_items.emplace_back(
        AclSubjectSnapshot(AclSubjectKind::OwnerUser, 0),
        domain::RightType::READ,
        domain::AccessType::ALLOW
    );
    AclSnapshot acl(std::move(acl_items));

    auto acl_snapshot = std::make_unique<AclSnapshot>(std::vector<AclItemSnapshot>());
    FileMetadataSnapshot metadata(1000, 0, 0, 1000, 2000, 0, std::move(acl_snapshot));
    auto file = std::make_unique<RegularFileSnapshot>(
        "test.txt", "/test.txt", std::move(metadata), "content"
    );

    auto root_acl = std::make_unique<AclSnapshot>(std::vector<AclItemSnapshot>());
    auto root_metadata = FileMetadataSnapshot(1000, 0, 0, 1000, 2000, 0, std::move(root_acl));
    auto root = std::make_unique<DirectorySnapshot>(
        "/", "/", std::move(root_metadata),
        std::vector<std::unique_ptr<FileObjectSnapshot>>()
    );

    std::vector<UserSnapshot> users;
    users.emplace_back(1000, "user1", 2000, std::vector<std::uint64_t>());

    std::vector<GroupSnapshot> groups;
    groups.emplace_back(2000, "group1", std::vector<std::uint64_t>(), std::vector<std::uint64_t>());

    FileSystemSnapshot snapshot(std::move(root), std::move(users), std::move(groups));

    std::string test_file = "/tmp/test_fs_snapshot.yaml";
    serializer.write(test_file, snapshot);

    REQUIRE(std::filesystem::exists(test_file));

    auto restored = serializer.read(test_file);

    REQUIRE(restored.getUsers().size() == 1);
    REQUIRE(restored.getGroups().size() == 1);
    REQUIRE(restored.getUsers()[0].getUid() == 1000);
    REQUIRE(restored.getGroups()[0].getGid() == 2000);

    std::filesystem::remove(test_file);
}

