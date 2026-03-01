#pragma once
#include <vector>

#include "result_cmdlets/icmdlet_result.h"
#include "../../cli_view.h"
#include "dto/rights/e_rights_dto.h"

namespace cmdlets {
/**
 * @class FileERightsResult
 * 
 * @brief результат выполнения команды эффективных прав доступа
 * 
 * содержит данные об эффективных правах доступа для одного или нескольких файлов
 */
class FileERightsResult : public ICmdletResult {
    std::vector<FileERightsDTO> dtos_;
    public:
    /**
     * @brief конструктор результата эффективных прав доступа
     * 
     * @param dtos вектор данных об эффективных правах доступа
     */
    FileERightsResult(std::vector<FileERightsDTO> dtos) : dtos_(std::move(dtos)) {}

    /**
     * @brief отрисовать результат эффективных прав доступа
     * 
     * @param view представление для отрисовки
     */
    void render(view::ICliView &view) const override {
        for (const auto& dto : dtos_) {
            view.showEffectiveRights(dto);
        }
    }
};
}