#pragma once

#include <memory>
#include <thread>

#include "app_base.h"
#include "../../mt/thread_pool.h"

class SafeQueue;
struct AtomicDiskUsageDTO;
struct DiskUsageDTO;
class FileSystemSnapshotDTO;

namespace domain {
class IDirectory;
class IFileObject;
}

namespace services {
class IKernelService;

/**
 * @class IFileSystemAppService
 * 
 * @brief интерфейс для сервиса работы с файловой системой на уровне приложения
 * 
 * предоставляет операции для получения информации о файловой системе
 */
class IFileSystemAppService {
public:
    /**
     * @brief деструктор интерфейса сервиса файловой системы
     */
    virtual ~IFileSystemAppService() = default;

    /**
     * @brief получить информацию об использовании диска
     * 
     * @return DiskUsageDTO данные об использовании диска
     */
    virtual DiskUsageDTO diskUsage() = 0;
};

/**
 * @class FileSystemAppService
 * 
 * @brief реализация сервиса работы с файловой системой на уровне приложения
 * 
 * использует многопоточность для подсчета использования диска
 */
class FileSystemAppService : public IFileSystemAppService, public AppServiceBase {
    std::shared_ptr<IKernelService> kernel;
    ThreadPool thread_pool;
public:
    /**
     * @brief конструктор сервиса файловой системы
     * 
     * @param kernel сервис ядра файловой системы
     * @param session сервис сессии
     */
    FileSystemAppService(std::shared_ptr<IKernelService> kernel,
        ISessionService& session) :
        AppServiceBase(session),
        kernel(std::move(kernel)),
        thread_pool(std::thread::hardware_concurrency()){}

    /**
     * @brief деструктор сервиса файловой системы
     */
    ~FileSystemAppService() override {
        if (thread_pool.isStarted()) {
            thread_pool.stop();
        }
    }

    /**
     * @brief получить информацию об использовании диска
     * 
     * @return DiskUsageDTO данные об использовании диска
     */
    DiskUsageDTO diskUsage() override;
private:
    /**
     * @brief обход файловой системы в однопоточном режиме
     * 
     * @param obj объект файловой системы
     * @param acc аккумулятор данных об использовании диска
     */
    void walkBasic(
        domain::IFileObject* obj,
        DiskUsageDTO& acc);

    /**
     * @brief реализация многопоточного обхода директории
     * 
     * @param dir директория для обхода
     * @param acc атомарный аккумулятор данных об использовании диска
     * @param threads вектор потоков
     */
    void walkMtImpl(domain::IDirectory* dir, AtomicDiskUsageDTO& acc,
        std::vector<std::thread>& threads);

    /**
     * @brief многопоточный обход директории
     * 
     * @param dir директория для обхода
     * @param acc атомарный аккумулятор данных об использовании диска
     * @param thread_pool пул потоков
     * @param depth глубина рекурсии
     */
    void walkMt(const domain::IDirectory* dir, AtomicDiskUsageDTO& acc, ThreadPool& thread_pool, int depth = 0);

    /**
     * @brief рабочий поток для обхода в ширину
     * 
     * @param queue очередь задач
     * @param acc атомарный аккумулятор данных об использовании диска
     * @param active_workers счетчик активных рабочих потоков
     */
    void bfsWorker(
        SafeQueue& queue,
        AtomicDiskUsageDTO& acc,
        std::atomic<std::size_t>& active_workers);
    };

}