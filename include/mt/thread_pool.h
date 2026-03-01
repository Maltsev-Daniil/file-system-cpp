#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

/**
 * @class ThreadPool
 * 
 * @brief пул потоков для параллельного выполнения задач
 * 
 * управляет набором потоков для выполнения задач в очереди
 */
class ThreadPool {
public:
    /**
     * @brief конструктор пула потоков
     * 
     * @param num_threads количество потоков в пуле
     */
    explicit ThreadPool(std::size_t num_threads);

    /**
     * @brief деструктор пула потоков
     */
    ~ThreadPool();

    /**
     * @brief запустить пул потоков
     */
    void start();

    /**
     * @brief остановить пул потоков
     */
    void stop();

    /**
     * @brief добавить задачу в очередь
     * 
     * @param task задача для выполнения
     */
    void addTask(std::function<void()> task);

    /**
     * @brief дождаться завершения всех задач
     */
    void wait();
    
    /**
     * @brief проверить запущен ли пул потоков
     * 
     * @return true если пул запущен, false иначе
     */
    bool isStarted() const { return started; }

private:
    /**
     * @brief функция выполнения задач в потоке
     */
    void run();

private:
    std::queue<std::function<void()>> tasks;
    std::vector<std::thread> threads;

    std::mutex mutex;
    std::condition_variable cv;

    std::atomic<std::size_t> active_tasks{0};
    bool stop_flag = false;
    bool started = false;

    std::size_t num_threads;
};