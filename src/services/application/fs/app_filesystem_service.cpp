#include "services/application/app_filesystem_service.h"

#include <future>
#include <queue>
#include <thread>
#include <chrono>

#include "domain/objects/directory.h"
#include "../../../../include/dto/fs/disk_usage_dto.h"
#include "dto/fs/atomic_disk_usage_dto.h"
#include "services/domain/kernel_service.h"
#include "domain/objects/directory_structure.h"
#include "domain/objects/inmem_file.h"

using namespace domain;

namespace services {

DiskUsageDTO FileSystemAppService::diskUsage()
{
    auto root = kernel->getRootDir();
    if (!root) {
        throw std::logic_error(
            "diskUsage: kernel dir is null");
    }
    AtomicDiskUsageDTO result;
    DiskUsageDTO result_basic;

    thread_pool.start();
    result.total_files.store(0);
    result.total_dirs.store(0);
    result.total_size.store(0);

    thread_pool.addTask([&]() {
        walkMt(root, result, thread_pool, 0);
    });
    thread_pool.wait();
    thread_pool.stop();

    // std::cout << "Basic: ";
    // long long total_sum_basic_us = 0;
    // double n_times = 500;
    //
    // for (size_t i = 0; i < n_times; i++) {
    //     result_basic = DiskUsageDTO{0, 0, 0};
    //     auto start_basic = std::chrono::system_clock::now();
    //     walkBasic(root, result_basic);
    //     auto end_basic = std::chrono::system_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_basic - start_basic);
    //     total_sum_basic_us += duration.count();
    // }
    // double avg_basic_ms = static_cast<double>(total_sum_basic_us) / n_times / 1000.0;
    // std::cout << avg_basic_ms << " ms" << std::endl;
    //
    // std::cout << "MT: ";
    // long long total_sum_mt_us = 0;
    //
    // thread_pool.start();
    // for (size_t i = 0; i < n_times; i++) {
    //     result.total_files.store(0);
    //     result.total_dirs.store(0);
    //     result.total_size.store(0);
    //
    //     auto start_mt = std::chrono::system_clock::now();
    //     thread_pool.addTask([&]() {
    //         walkMt(root, result, thread_pool, 0);
    //     });
    //     thread_pool.wait();
    //     auto end_mt = std::chrono::system_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_mt - start_mt);
    //     total_sum_mt_us += duration.count();
    // }
    // thread_pool.stop();
    // double avg_mt_ms = static_cast<double>(total_sum_mt_us) / n_times / 1000.0;
    // std::cout << avg_mt_ms << " ms" << std::endl;
    //
    return {
        static_cast<std::size_t>(result.total_files),
        static_cast<std::size_t>(result.total_dirs),
        static_cast<std::size_t>(result.total_size)};
}

void FileSystemAppService::walkBasic(
    domain::IFileObject* obj,
    DiskUsageDTO& acc)
{
    if (auto* dir = dynamic_cast<IDirectory*>(obj)) {
        acc.total_dirs += 1;
        for (auto entry : dir->getDirectoryStructure()->getObjs()) {
            walkBasic(entry, acc);
        }
    }
    else {
        acc.total_files += 1;
        acc.total_size += obj->getInMemoryFile()->getSize();
    }
}

// void FileSystemAppService::bfsWorker(
//     DirectoryQueue& queue,
//     AtomicDiskUsageDTO& acc,
//     std::atomic<std::size_t>& active_workers)
// {
//     while (true) {
//         const IDirectory* dir = nullptr;
//
//         if (!queue.try_pop(dir)) {
//             if (active_workers.load(std::memory_order_relaxed) == 0) {
//                 break;
//             }
//             std::this_thread::yield();
//             continue;
//         }
//
//         active_workers.fetch_add(1, std::memory_order_relaxed);
//
//         DiskUsageDTO local;
//
//         for (const auto& obj : dir->getDirectoryStructure()->getObjs()) {
//             if (obj->isDirectory()) {
//                 queue.push(static_cast<const IDirectory*>(obj));
//                 local.total_dirs++;
//             } else {
//                 local.total_files++;
//                 local.tsize += obj->getInMemoryFile()->getSize();
//             }
//         }
//
//         acc.merge(local.files, local.dirs, local.size);
//
//         active_workers.fetch_sub(1, std::memory_order_relaxed);
//     }
// }

// void FileSystemAppService::walkMtImpl(
//     domain::IDirectory* dir,
//     AtomicDiskUsageDTO& acc,
//     std::vector<std::thread>& threads) {
//     for (const auto& obj : dir->getDirectoryStructure()->getObjs()) {
//         if (auto* dir_got = dynamic_cast<IDirectory*>(obj)) {
//             if (threads.size() < threads.capacity()) {
//                 threads.push_back(std::thread(
//                     [&](){walkMtImpl(dir_got, acc, threads);}));
//             } else {
//                 walkMtImpl(dir_got, acc, threads);
//             }
//         } else {
//             acc.total_files.fetch_add(1);
//         }
//     }
//     threads.pop_back();
// }

void FileSystemAppService::walkMt(const domain::IDirectory *dir, AtomicDiskUsageDTO &acc
    , ThreadPool& thread_pool, int depth) {

    DiskUsageDTO local{0, 1, 0};
    std::vector<IDirectory*> subdirs;

    for (const auto& obj : dir->getDirectoryStructure()->getObjs()) {
        if (auto* dir_got = dynamic_cast<IDirectory*>(obj)) {
            subdirs.push_back(dir_got);
        } else {
            local.total_files++;
            local.total_size += obj->getInMemoryFile()->getSize();
        }
    }

    acc.merge(local.total_files, local.total_dirs, local.total_size);

    if (depth < 3) {
        for (auto* subdir : subdirs) {
            thread_pool.addTask([this, subdir, &acc, &thread_pool, depth]() {
                walkMt(subdir, acc, thread_pool, depth + 1);
            });
        }
    } else {
        for (auto* subdir : subdirs) {
            DiskUsageDTO subtree_local{0, 0, 0};
            walkBasic(subdir, subtree_local);
            acc.merge(subtree_local.total_files, subtree_local.total_dirs, subtree_local.total_size);
        }
    }
}


}
