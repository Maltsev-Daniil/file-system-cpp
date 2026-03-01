#pragma once

struct DiskUsageDTO {
    std::size_t total_files = 0;
    std::size_t total_dirs  = 0;
    std::size_t total_size  = 0; 
};