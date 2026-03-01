#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "dto/fs/disk_usage_dto.h"

TEST_CASE("DiskUsageDTO default values") {
    DiskUsageDTO dto;
    REQUIRE(dto.total_files == 0);
    REQUIRE(dto.total_dirs == 0);
    REQUIRE(dto.total_size == 0);
}

TEST_CASE("DiskUsageDTO set values") {
    DiskUsageDTO dto;
    dto.total_files = 10;
    dto.total_dirs = 5;
    dto.total_size = 1024;

    REQUIRE(dto.total_files == 10);
    REQUIRE(dto.total_dirs == 5);
    REQUIRE(dto.total_size == 1024);
}

