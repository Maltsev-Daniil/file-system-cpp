#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "mt/thread_pool.h"

#include <atomic>
#include <chrono>
#include <set>
#include <thread>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <exception>

using namespace std::chrono_literals;

TEST_CASE("ThreadPool basic construction and destruction") {
    ThreadPool pool(4);
    REQUIRE_FALSE(pool.isStarted());
}

TEST_CASE("ThreadPool start and stop") {
    ThreadPool pool(4);

    pool.start();
    REQUIRE(pool.isStarted());

    pool.start();

    pool.stop();
    REQUIRE_FALSE(pool.isStarted());

    pool.start();
    // потом неявно вызываем деструктор
}

TEST_CASE("ThreadPool executes single task") {
    ThreadPool pool(2);
    pool.start();

    std::atomic<int> counter{0};

    pool.addTask([&counter]() {
        counter.fetch_add(1);
    });

    pool.wait();
    pool.stop();

    REQUIRE(counter.load() == 1);
}

TEST_CASE("ThreadPool executes multiple tasks") {
    ThreadPool pool(4);
    pool.start();

    std::atomic<int> counter{0};
    const int num_tasks = 100;

    for (int i = 0; i < num_tasks; ++i) {
        pool.addTask([&counter]() {
            counter.fetch_add(1);
        });
    }

    pool.wait();
    pool.stop();

    REQUIRE(counter.load() == num_tasks);
}

TEST_CASE("ThreadPool uses multiple threads - thread IDs are different") {
    const std::size_t num_threads = 4;
    ThreadPool pool(num_threads);
    pool.start();

    std::mutex ids_mutex;
    std::set<std::thread::id> thread_ids;
    std::atomic<int> tasks_completed{0};

    const int num_tasks = 100;

    for (int i = 0; i < num_tasks; ++i) {
        pool.addTask([&]() {

            {
                std::lock_guard lock(ids_mutex);
                thread_ids.insert(std::this_thread::get_id());
            }


            std::this_thread::sleep_for(1ms);

            tasks_completed.fetch_add(1);
        });
    }

    pool.wait();
    pool.stop();

    REQUIRE(tasks_completed.load() == num_tasks);


    INFO("Number of unique thread IDs used: " << thread_ids.size());
    REQUIRE(thread_ids.size() > 1);
    REQUIRE(thread_ids.size() <= num_threads);
}

TEST_CASE("ThreadPool parallel execution is faster than sequential", "[benchmark]") {
    const std::size_t num_threads = std::thread::hardware_concurrency();
    INFO("Hardware concurrency: " << num_threads << " threads");

    const int num_tasks = 32;
    const auto work_duration = 10ms;


    auto seq_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_tasks; ++i) {
        std::this_thread::sleep_for(work_duration);
    }
    auto seq_end = std::chrono::high_resolution_clock::now();
    auto seq_duration = std::chrono::duration_cast<std::chrono::milliseconds>(seq_end - seq_start);


    ThreadPool pool(num_threads);
    pool.start();

    std::atomic<int> completed{0};

    auto par_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_tasks; ++i) {
        pool.addTask([&]() {
            std::this_thread::sleep_for(work_duration);
            completed.fetch_add(1);
        });
    }
    pool.wait();
    auto par_end = std::chrono::high_resolution_clock::now();
    auto par_duration = std::chrono::duration_cast<std::chrono::milliseconds>(par_end - par_start);

    pool.stop();

    REQUIRE(completed.load() == num_tasks);

    double speedup = static_cast<double>(seq_duration.count()) / par_duration.count();

    WARN("Performance results - Cores: " << num_threads
     << ", Tasks: " << num_tasks
     << ", Seq: " << seq_duration.count() << "ms"
     << ", Par: " << par_duration.count() << "ms"
     << ", Speedup: " << std::fixed << std::setprecision(1) << speedup << "x");

    REQUIRE(par_duration < seq_duration);


    REQUIRE(speedup > 1.5);
}

TEST_CASE("ThreadPool handles CPU-intensive work in parallel") {
    const std::size_t num_threads = std::thread::hardware_concurrency();
    ThreadPool pool(num_threads);
    pool.start();

    std::atomic<long long> total_sum{0};
    const int num_tasks = num_threads * 2;
    const long long iterations_per_task = 1000000;

    std::mutex ids_mutex;
    std::set<std::thread::id> thread_ids;

    auto start = std::chrono::high_resolution_clock::now();

    for (int t = 0; t < num_tasks; ++t) {
        pool.addTask([&, t]() {

            {
                std::lock_guard lock(ids_mutex);
                thread_ids.insert(std::this_thread::get_id());
            }


            long long local_sum = 0;
            for (long long i = 0; i < iterations_per_task; ++i) {
                local_sum += (i * t) % 17;
            }
            total_sum.fetch_add(local_sum);
        });
    }

    pool.wait();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    pool.stop();

    WARN("Thread pool test - "
     << "Cores: " << num_threads
     << ", Workers: " << thread_ids.size()
     << ", Tasks: " << num_tasks
     << ", Iters/task: " << iterations_per_task
     << ", Sum: " << total_sum.load()
     << ", Time: " << duration.count() << "ms");

    REQUIRE(thread_ids.size() >= std::min(num_threads, static_cast<std::size_t>(num_tasks)));
}

TEST_CASE("ThreadPool atomic counter stress test") {
    const std::size_t num_threads = 8;
    ThreadPool pool(num_threads);
    pool.start();

    std::atomic<int> counter{0};
    const int increments_per_task = 10000;
    const int num_tasks = 100;

    for (int t = 0; t < num_tasks; ++t) {
        pool.addTask([&]() {
            for (int i = 0; i < increments_per_task; ++i) {
                counter.fetch_add(1);
            }
        });
    }

    pool.wait();
    pool.stop();


    REQUIRE(counter.load() == num_tasks * increments_per_task);
}

TEST_CASE("ThreadPool wait blocks until all tasks complete") {
    ThreadPool pool(4);
    pool.start();

    std::atomic<int> completed{0};
    const int num_tasks = 10;

    for (int i = 0; i < num_tasks; ++i) {
        pool.addTask([&]() {
            std::this_thread::sleep_for(50ms);
            completed.fetch_add(1);
        });
    }


    pool.wait();

    REQUIRE(completed.load() == num_tasks);

    pool.stop();
}

TEST_CASE("ThreadPool can be restarted") {
    ThreadPool pool(2);

    std::atomic<int> counter{0};


    pool.start();
    pool.addTask([&]() { counter.fetch_add(1); });
    pool.wait();
    pool.stop();

    REQUIRE(counter.load() == 1);

    pool.start();
    pool.addTask([&]() { counter.fetch_add(10); });
    pool.wait();
    pool.stop();

    REQUIRE(counter.load() == 11);
}

TEST_CASE("ThreadPool cannot add task if not running") {
    ThreadPool pool(2);
    REQUIRE_THROWS_AS(pool.addTask([&]() {}), std::runtime_error);
}