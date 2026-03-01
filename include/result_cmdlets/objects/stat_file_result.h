#pragma once
#include <algorithm>

#include "../icmdlet_result.h"
#include "../../cli_view.h"

struct StatFileDTO;

namespace cmdlets {
/**
 * @class StatFileResult
 * 
 * @brief результат выполнения команды статистики файла
 * 
 * содержит данные о статистике одного или нескольких файлов
 */
class StatFileResult : public ICmdletResult {
    std::vector<StatFileDTO> dtos_;
public:
    /**
     * @brief конструктор результата статистики файла
     * 
     * @param dtos вектор данных о статистике файлов
     */
    StatFileResult(std::vector<StatFileDTO> dtos) : dtos_(std::move(dtos)) {}

    /**
     * @brief отрисовать результат статистики файла
     * 
     * @param view представление для отрисовки
     */
    void render(view::ICliView &view) const override {
        for (const auto& dto : dtos_) {
            view.showFileStat(dto);
        }
    }
};
}