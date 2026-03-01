#pragma once
#include <algorithm>

#include "icmdlet_result.h"
#include "../cli_view.h"

struct DiskUsageDTO;

namespace cmdlets {
/**
 * @class DiskUsageResult
 * 
 * @brief результат выполнения команды использования диска
 * 
 * содержит данные об использовании дискового пространства
 */
class DiskUsageResult : public ICmdletResult {
    DiskUsageDTO dto_;
    public:
    /**
     * @brief конструктор результата использования диска
     * 
     * @param dto данные об использовании диска
     */
    DiskUsageResult(DiskUsageDTO dto) : dto_(std::move(dto)) {}

    /**
     * @brief отрисовать результат использования диска
     * 
     * @param view представление для отрисовки
     */
    void render(view::ICliView &view) const override {
        view.showDiskUsage(dto_);
    }
};
}