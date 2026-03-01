#pragma once
#include <algorithm>

#include "../icmdlet_result.h"
#include "../../cli_view.h"

struct DirContentDTO;

namespace cmdlets {
/**
 * @class DirContentResult
 * 
 * @brief результат выполнения команды содержимого директории
 * 
 * содержит данные о содержимом одной или нескольких директорий
 */
class DirContentResult : public ICmdletResult {
    std::vector<DirContentDTO> dtos_;
    public:
    /**
     * @brief конструктор результата содержимого директории
     * 
     * @param dtos вектор данных о содержимом директорий
     */
    DirContentResult(std::vector<DirContentDTO> dtos) : dtos_(std::move(dtos)) {}

    /**
     * @brief отрисовать результат содержимого директории
     * 
     * @param view представление для отрисовки
     */
    void render(view::ICliView &view) const override {
        for (const auto& dto : dtos_) {
            view.showDirectoryContent(dto);
        }
    }
};
}