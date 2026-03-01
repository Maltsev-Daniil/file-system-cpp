#pragma once

#include <memory>
#include <string>
#include <vector>

namespace services {
class ISessionService;

class IKernelService;
class IPathToAbs;

// wildcard только в последнем компоненте!!!!
/**
 * @class IPathPatternResolver
 * 
 * @brief интерфейс для резолвера путей и паттернов
 * 
 * разрешает пути и паттерны с подстановочными знаками в абсолютные пути
 */
class IPathPatternResolver {
public:
    /**
     * @brief деструктор интерфейса резолвера путей
     */
    virtual ~IPathPatternResolver() = default;

    /**
     * @brief разрешить паттерн с подстановочными знаками
     * 
     * @param absolute_pattern абсолютный паттерн для разрешения
     * @return std::vector<std::string> вектор разрешенных путей
     */
    virtual std::vector<std::string> resolveGlob(
        const std::string& absolute_pattern
    ) const = 0;

    /**
     * @brief преобразовать путь в абсолютный
     * 
     * @param path путь для преобразования
     * @return std::string абсолютный путь
     */
    virtual std::string pathToAbsolute(
        const std::string& path
    ) const = 0;
};

/**
 * @class PathPatternResolver
 * 
 * @brief реализация резолвера путей и паттернов
 * 
 * разрешает относительные пути и паттерны с использованием текущей сессии
 */
class PathPatternResolver : public IPathPatternResolver {
public:
    /**
     * @brief конструктор резолвера путей
     * 
     * @param kernel_service сервис ядра файловой системы
     * @param session сервис сессии
     */
    PathPatternResolver(IKernelService& kernel_service,
        ISessionService& session)
        : session(session),
    kernel_service(kernel_service) {}

    /**
     * @brief разрешить паттерн с подстановочными знаками
     * 
     * @param absolute_pattern абсолютный паттерн для разрешения
     * @return std::vector<std::string> вектор разрешенных путей
     */
    std::vector<std::string> resolveGlob(
        const std::string& absolute_pattern
    ) const override;

    /**
     * @brief преобразовать путь в абсолютный
     * 
     * @param path путь для преобразования
     * @return std::string абсолютный путь
     */
    std::string pathToAbsolute(
        const std::string& path
    ) const noexcept override;

private:
    IKernelService& kernel_service;
    ISessionService& session;
};

} // namespace services


