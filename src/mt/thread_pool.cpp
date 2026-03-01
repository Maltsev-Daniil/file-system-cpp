//
// Created by nitsir on 1/7/26.
//

#include "../../include/mt/thread_pool.h"

ThreadPool::ThreadPool(std::size_t num_threads)
    : num_threads(num_threads) {}

ThreadPool::~ThreadPool() {
    if (started) {
       stop();
    }
}

void ThreadPool::start() {
    if (started) {
        return;
    }

    started = true;
    stop_flag = false;

    threads.reserve(num_threads);
    for (std::size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([this] {
            run();
        });
    }
}

void ThreadPool::stop() {
    {
        std::lock_guard lock(mutex);
        stop_flag = true;
    }

    cv.notify_all();

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    threads.clear();
    started = false;
}

void ThreadPool::addTask(std::function<void()> task) {
    std::lock_guard lock(mutex);
    if (!isStarted()) {
        throw std::runtime_error("addTask: pool not started");
    }
    tasks.push(std::move(task));
    active_tasks.fetch_add(1);
    cv.notify_one();
}

void ThreadPool::wait() {
    std::unique_lock lock(mutex);
    cv.wait(lock, [&] {
        return tasks.empty() && active_tasks.load() == 0;
    });
}

void ThreadPool::run() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock lock(mutex);

            cv.wait(lock, [&] {
                return stop_flag || !tasks.empty();
            });

            if (stop_flag && tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();

        active_tasks.fetch_sub(1);
        cv.notify_all();
    }
}