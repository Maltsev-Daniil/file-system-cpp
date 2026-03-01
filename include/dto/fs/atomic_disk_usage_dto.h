#pragma once
#include <atomic>

struct AtomicDiskUsageDTO {
    std::atomic<std::size_t> total_files = 0;
    std::atomic<std::size_t> total_dirs  = 0;
    std::atomic<std::size_t> total_size  = 0;

    void merge(std::size_t files,
        std::size_t dirs,
        std::size_t size) {
        total_files.fetch_add(files);
        total_dirs.fetch_add(dirs);
        total_size.fetch_add(size);
    }
};
